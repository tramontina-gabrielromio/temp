const express = require('express')
const router = express.Router()
const mongoose = require('mongoose')
const passport = require("passport")
const {logged} = require("../helpers/logado")
const localStrategy = require("passport-local").Strategy
const bcrypt = require("bcryptjs")
const session = require('express-session');
const path = require("path")
const { execFile } = require('child_process');
const { exec } = require('child_process');
const {spawn} = require('child_process');
require("../models/Ferramenta")
const Ferramenta = mongoose.model("ferramentas")
require("../models/Evento")
const Evento = mongoose.model("eventos")
var userLogged = require("../routes/userLogged");
require("../models/Usuario")
const Usuario = mongoose.model("usuarios")
require("../models/DadosTeste")
const DadosTeste = mongoose.model("dadosTestes")
require("../models/Configuracoes")
const Configuracao = mongoose.model("configuracoes")
const wifi = require('node-wifi')
var nodemailer = require('nodemailer')
const ObjectsToCsv = require('objects-to-csv')
const schedule = require('node-schedule')
const usb = require('usb')
const ngrok = require('ngrok')
const child_process_dateTime = require("child_process");
const child_restoreSystem = require("child_process");
const dateFormat = require("dateformat");
const sgMail = require('@sendgrid/mail')
const fs = require("fs")
require('dotenv').config({path:'./sendgrid.env'});

var redesNeuraisCarregadas = 0
var FuncSenha = require("./senha");
var senha = ""
var abrindoGV = 0
var fechandoGV = 0
var bloqueiaAbertura = 0
var ItensErrados = [0, 0, 0, 0, 0, 0, 0, 0] //404 indica estado desconhecido
var auxFGVSemRedeNeural = 1
var redeWifiOk = 0
var wifiLevel = 0
var emailTramontina = 'sistemainteligente.pro@tramontina.com' //Configura e-mail interno de usuario Tramontina para envio de registros
var cargaBateria = 0
var BateriaCarregando = 0
var BateriaCarregada = 0
var arrayBateria = []
var auxGavetaAberta = 0

var timerRelatorioDiario
var timerAdiaEmail

try{ //Verifica numero de série do organizador
  var serialOrganizador = require("./serial");
}catch (error){
  console.log('Error: ' + error)
  serialOrganizador = ''
}

try{ //Verifica versão de software do organizador
  var versaoSoftware = require("./versao");
}catch (error){
  console.log('Error: ' + error)
  versaoSoftware = ''
}

//Variáveis referentes à childprocess
var childCam1
var childCam2
var childRedeNeural
var childVerificacao

//Variáveis auxiliares para testes das gavetas
var contRepeticoesGVEspecifica = 0
var contRepeticoesTodasGVs = 0
var numRepeticoesGVEspecifica = 0
var numRepeticoesTodasGVs = 0
var pausaTestesGavetas = 0
var paraTestesGavetas = 0
var testesGavetasAndamento = 0
var gavetaEmTeste = 1
var gavetaEmMovimento = ''
var segDecorridoGVEspecifica = ''
var segDecorridoTodasGVs = ''
var ObjectId = require('mongodb').ObjectID

//Alterar ip
/*var set_ip_address = require('set-ip-address')

var wlan0 = {
  interface: 'wlan0',
  ip_address: '10.0.0.198',
  prefix: 23,
  gateway: '10.0.0.1',
  nameservers: ['8.8.8.8'],
  optional: true
}

set_ip_address.configure([wlan0]).then(() => {
  console.log('done writing config files')
  set_ip_address.restartService()
})*/

//Temporizadores
/*function Timer(callback, delay) {
  var timerId;
  var start;
  var remaining = delay;

  this.pause = function () {
    window.clearTimeout(timerId);
    remaining -= new Date() - start;
  };

  var resume = function () {
    start = new Date();
	timerId = window.setTimeout(function () {
      remaining = delay;
      resume();
      callback();
    }, remaining);
  }
  this.resume = resume;

  this.reset = function () {
    remaining = delay;
  };
}

var timer = new Timer(function () {
  console.log("Chamou a função!");
}, 3000);
timer.resume();*/

//Carrega redes neurais durante a inicialização
var childRedeNeural
inicializaRedesNeurais()
function inicializaRedesNeurais() {
  console.log('Carregando redes neurais...')
  childRedeNeural = spawn('/home/tramontina/Downloads/Interface/RedesNeurais', [], {detached: false});
  childRedeNeural.on('exit', (code) => {
      console.log('Uma das redes neurais parou de responder...')
  });
  childRedeNeural.stdout.on('data', (data) => {
      if (data[0]==49){
        redesNeuraisCarregadas = 1
        console.log("Redes neurais carregadas com sucesso!")
        //ExecuteChromium(); //Abre o navegador quando a inicialização das redes neurais for concluida
      }
  });
  childRedeNeural.stderr.on('data', (data) => {
      //console.log(data);
  });
}

verificaGavetasInicial() //Verifica gavetas ao ligar o organizador, evitando possíveis falhas
function verificaGavetasInicial(){
	execFile('/home/tramontina/Downloads/Interface/Gavetas', ['GAVETAS'], (error, stdout, stderr) => {
	  if (error) {
		console.error('error: ' + error);
		return;
		}
	  if (stderr) {
		console.error('stderr: ' + stderr);
		return;
	  }
	  console.log('stdout: ' + stdout);
	});
}

//Altera data e hora do sistema
//setDateTime('3/31/2022 14:10:00') //mm/dd/yyyy HH:MM:ss
function setDateTime(dateTime) {
  //let sudo = 'sudo ';
  child_process_dateTime.exec(`timedatectl set-ntp false`);
  try{
    child_process_dateTime.exec(`timedatectl set-time "${dateFormat(dateTime, 'yyyy-mm-dd HH:MM:ss')}"`);
  }catch (error){
    console.log('Error: ' + error)
  }
  child_process_dateTime.exec(`hwclock -w --utc`);
}

//Configura leitura de pendrive
//const drivelist = require('drivelist')
//console.log("Drives encontrados: " + drivelist.list()) 

//Configura acesso remoto
//ngrokConnect()
ngrok.disconnect(); //Inicia ngrok desconectado de todas as portas
ngrok.kill(); //Mata todos os processos do ngrok ao iniciar

//Carrega configurações salvas
var configuracaoAtual = new Array
Configuracao.findOne({_id:'000000000000000000000001'}).lean().populate("configuracoes").then((configuracoes) => {
  configuracaoAtual = {
    fechamento: configuracoes.fechamento,
    cartao: configuracoes.cartao,
    desligamento: configuracoes.desligamento,
    encerrarSessao: configuracoes.encerrarSessao,
    email: configuracoes.email,
    email2: configuracoes.email2,
    email3: configuracoes.email3,
    identificacao: configuracoes.identificacao,
    dataRelatorio: configuracoes.dataRelatorio
  }
  if (dataAtual!=configuracoes.dataRelatorio){
    geraRelatorio('registros')
    console.log('Gera relatorio e-mail 1')
  }
  else{
    console.log('Definindo envio de e-mail para as 0h00')
    try{
      timerRelatorioDiario.cancel()
    }catch(err){
      //console.log(err)
    }
    try{
      timerAdiaEmail.cancel()
    }catch(err){
      //console.log(err)
    }
    timerRelatorioDiario = schedule.scheduleJob('0 0 * * *', function(){
      geraRelatorio('registros')
      console.log('Gera relatorio e-mail 2')
    })
  }
}).catch((err) => {
  console.log('Erro ao carregar as configurações: ' + err)
  try{
    timerRelatorioDiario.cancel()
  }catch(err){
    //console.log(err)
  }
  try{
    timerAdiaEmail.cancel()
  }catch(err){
    //console.log(err)
  }
  timerRelatorioDiario = schedule.scheduleJob('0 0 * * *', function(){
	geraRelatorio('registros')
  console.log('Gera relatorio e-mail 3')
  })
  configuracaoAtual = {
    fechamento: 0,
    cartao: 0,
    desligamento: 0,
    encerrarSessao: 0,
    email: '',
    email2: '',
    email3: '',
    identificacao: '',
    dataRelatorio: ''
  }
})

//Configuração para envio de relatório através de e-mail
var today=new Date();
var dataAtual = String(today.getDate() + '-' + (today.getMonth()+1) + '-' + today.getFullYear())
//console.log(dataAtual)
//var min = today.getMinutes()
//var hora = today.getHours()

/*var remetente = nodemailer.createTransport({
  host:'smtp.office365.com',
  service:'smtp.office365.com',
  port:587,
  secure:false,
  auth:{
    user:'smart_manager4.0@outlook.com',
    pass:''
  }
})*/
/*
var remetente = nodemailer.createTransport({
  host:'smtp.gmail.com',
  service:'smtp.gmail.com',
  port:587,
  secure:false,
  auth:{
    user:'gabriel.romio@tramontina.com',
    pass:''
  }
})*/

//geraRelatorio('relatorio')
/*Evento.updateMany({enviado:0}, {enviado:1}).then((eventos) => { //Atualiza multiplos eventos
  console.log("Eventos atualizados")
}).catch((err) => {
  console.log(err)
})*/
/*
testeGeraRelatorio()
function testeGeraRelatorio(data){
  var periodo = ['', '', '', '', '', '', '', '']
  const currentDate=new Date();
  periodo[0]=currentDate.toLocaleDateString()
  periodo[1]=(new Date(currentDate.setDate(currentDate.getDate()-1))).toLocaleDateString()
  periodo[2]=(new Date(currentDate.setDate(currentDate.getDate()-1))).toLocaleDateString()
  periodo[3]=(new Date(currentDate.setDate(currentDate.getDate()-1))).toLocaleDateString()
  periodo[4]=(new Date(currentDate.setDate(currentDate.getDate()-1))).toLocaleDateString()
  periodo[5]=(new Date(currentDate.setDate(currentDate.getDate()-1))).toLocaleDateString()
  periodo[6]=(new Date(currentDate.setDate(currentDate.getDate()-1))).toLocaleDateString()
  periodo[7]=(new Date(currentDate.setDate(currentDate.getDate()-1))).toLocaleDateString()
  //console.log(periodo)
  //if (redeWifiOk==1){
    //Envia todos os dados referentes aos últimos 7 dias, ferramentas que não foram devolvidas, ou eventos que ainda não foram enviados
    Evento.find({$or:[
      {enviado:0}, {dataDevolucao:"-"}, {horaDevolucao:"-"}, 
      {dataRetirada: periodo[0]}, {dataDevolucao: periodo[0]},
      {dataRetirada: periodo[1]}, {dataDevolucao: periodo[1]},
      {dataRetirada: periodo[2]}, {dataDevolucao: periodo[2]},
      {dataRetirada: periodo[3]}, {dataDevolucao: periodo[3]},
      {dataRetirada: periodo[4]}, {dataDevolucao: periodo[4]},
      {dataRetirada: periodo[5]}, {dataDevolucao: periodo[5]},
      {dataRetirada: periodo[6]}, {dataDevolucao: periodo[6]},
      {dataRetirada: periodo[7]}, {dataDevolucao: periodo[7]}
    ]})
    .select('referencia descricao gaveta dataRetirada horaRetirada dataDevolucao horaDevolucao usuario setor -_id')
    .lean().populate("eventos").sort({_id: "asc"}).then((eventos) => {
      //console.log(eventos)
      if(eventos.length>0){
        const csv = new ObjectsToCsv(eventos);
        console.log('Gerando relatorio...')
        csv.toDisk('relatorio.csv').then(function(){
          console.log('Iniciando envio de e-mail...')
          //enviaEmail(data, 1)
        })
      }
      //else{
      //  enviaEmail(data, 0)
      //}
    }).catch((err) => {
      console.log(err)
    })
  //}
  //else{
  //  console.log('Wifi não conectado... adiando envio de e-mail')
  //  adiaEnvioEmail()
  //}
}*/

function geraRelatorio(data){
  var periodo = ['', '', '', '', '', '', '', '']
  const currentDate=new Date();
  periodo[0]=currentDate.toLocaleDateString()
  periodo[1]=(new Date(currentDate.setDate(currentDate.getDate()-1))).toLocaleDateString()
  periodo[2]=(new Date(currentDate.setDate(currentDate.getDate()-1))).toLocaleDateString()
  periodo[3]=(new Date(currentDate.setDate(currentDate.getDate()-1))).toLocaleDateString()
  periodo[4]=(new Date(currentDate.setDate(currentDate.getDate()-1))).toLocaleDateString()
  periodo[5]=(new Date(currentDate.setDate(currentDate.getDate()-1))).toLocaleDateString()
  periodo[6]=(new Date(currentDate.setDate(currentDate.getDate()-1))).toLocaleDateString()
  periodo[7]=(new Date(currentDate.setDate(currentDate.getDate()-1))).toLocaleDateString()
  if (redeWifiOk==1){
    //Envia todos os dados referentes aos últimos 7 dias, ferramentas que não foram devolvidas, ou eventos que ainda não foram enviados
    Evento.find({$or:[
      {enviado:0}, {dataDevolucao:"-"}, {horaDevolucao:"-"}, 
      {dataRetirada: periodo[0]}, {dataDevolucao: periodo[0]},
      {dataRetirada: periodo[1]}, {dataDevolucao: periodo[1]},
      {dataRetirada: periodo[2]}, {dataDevolucao: periodo[2]},
      {dataRetirada: periodo[3]}, {dataDevolucao: periodo[3]},
      {dataRetirada: periodo[4]}, {dataDevolucao: periodo[4]},
      {dataRetirada: periodo[5]}, {dataDevolucao: periodo[5]},
      {dataRetirada: periodo[6]}, {dataDevolucao: periodo[6]},
      {dataRetirada: periodo[7]}, {dataDevolucao: periodo[7]}
    ]}/*{$or:[{dataRetirada:data}, {dataDevolucao:data}]}*/).select('referencia descricao gaveta dataRetirada horaRetirada dataDevolucao horaDevolucao usuario setor -_id')
    .lean().populate("eventos").sort({_id: "asc"}).then((eventos) => {
      if(eventos.length>0){
        const csv = new ObjectsToCsv(eventos);
        console.log('Gerando relatorio...')
        csv.toDisk('relatorio.csv').then(function(){
          console.log('Iniciando envio de e-mail...')
          enviaEmail(data, 1)
        })
      }
      else{
        enviaEmail(data, 0)
      }
    }).catch((err) => {
      console.log(err)
    })
  }
  else{
    console.log('Wifi não conectado... adiando envio de e-mail')
    adiaEnvioEmail()
  }
}

//geraRelatorioTramontina('registros')

function geraRelatorioTramontina(data){
  var periodo = ['', '', '', '', '', '', '', '']
  const currentDate=new Date();
  periodo[0]=currentDate.toLocaleDateString()
  periodo[1]=(new Date(currentDate.setDate(currentDate.getDate()-1))).toLocaleDateString()
  periodo[2]=(new Date(currentDate.setDate(currentDate.getDate()-1))).toLocaleDateString()
  periodo[3]=(new Date(currentDate.setDate(currentDate.getDate()-1))).toLocaleDateString()
  periodo[4]=(new Date(currentDate.setDate(currentDate.getDate()-1))).toLocaleDateString()
  periodo[5]=(new Date(currentDate.setDate(currentDate.getDate()-1))).toLocaleDateString()
  periodo[6]=(new Date(currentDate.setDate(currentDate.getDate()-1))).toLocaleDateString()
  periodo[7]=(new Date(currentDate.setDate(currentDate.getDate()-1))).toLocaleDateString()
  //if (redeWifiOk==1){
    //Envia todos os dados referentes aos últimos 7 dias, ferramentas que não foram devolvidas, ou eventos que ainda não foram enviados
    Evento.find({$or:[
      {enviado:0}, {dataDevolucao:"-"}, {horaDevolucao:"-"}, 
      {dataRetirada: periodo[0]}, {dataDevolucao: periodo[0]},
      {dataRetirada: periodo[1]}, {dataDevolucao: periodo[1]},
      {dataRetirada: periodo[2]}, {dataDevolucao: periodo[2]},
      {dataRetirada: periodo[3]}, {dataDevolucao: periodo[3]},
      {dataRetirada: periodo[4]}, {dataDevolucao: periodo[4]},
      {dataRetirada: periodo[5]}, {dataDevolucao: periodo[5]},
      {dataRetirada: periodo[6]}, {dataDevolucao: periodo[6]},
      {dataRetirada: periodo[7]}, {dataDevolucao: periodo[7]}
    ]}/*{$or:[{dataRetirada:data}, {dataDevolucao:data}]}*/).select('referencia descricao gaveta dataRetirada horaRetirada dataDevolucao horaDevolucao -_id')
    .lean().populate("eventos").sort({_id: "asc"}).then((eventos) => {
      if(eventos.length>0){
        const csv = new ObjectsToCsv(eventos);
        console.log('Gerando relatorio...')
        csv.toDisk('relatorioTramontina.csv').then(function(){
          console.log('Iniciando envio de e-mail...')
          enviaEmailTramontina(data, 1)
        })
      }
      else{
        enviaEmailTramontina(data, 0)
      }
    }).catch((err) => {
      console.log(err)
    })
  //}
  //else{
  //  console.log('Wifi não conectado... adiando envio de e-mail')
  //  adiaEnvioEmail()
  //}
}

function enviaEmailTramontina(data, novosRegistros){
  var email
  if(novosRegistros==1){
    pathToAttachment = `./relatorioTramontina.csv`;
    attachment = fs.readFileSync(pathToAttachment).toString("base64");
    email = {
      to: emailTramontina,
      from: 'sistemainteligente.pro@tramontina.com',
      subject:'Relatório diário SmartManager4.0',
      text: 'Relatório SmartManager4.0\nNúmero de série: ' + serialOrganizador,
      attachments: [
        {
          content: attachment,
          filename: data + '.csv',
          type: 'application/csv',
          disposition: 'attachment'
        },
      ],
    }
  }
  else{
    email = {
      to: emailTramontina,
      from: 'sistemainteligente.pro@tramontina.com',
      subject:'Relatório diário SmartManager4.0',
      text: 'Relatório SmartManager4.0\nNúmero de série: ' + serialOrganizador + '\nNenhum registro novo.'
    }
  }
  sgMail
    .send(email)
    .then(() => {
      console.log('Email Tramontina enviado com sucesso')
    })
    .catch((error) => {
      console.log('Erro ao enviar email: ' + error);
    })
}

sgMail.setApiKey(process.env.SENDGRID_API_KEY)

function enviaEmail(data, novosRegistros){
  var email
  if(novosRegistros==1){
    pathToAttachment = `./relatorio.csv`;
    attachment = fs.readFileSync(pathToAttachment).toString("base64");
    email = {
      to: [configuracaoAtual.email, configuracaoAtual.email2, configuracaoAtual.email3],
      from: 'sistemainteligente.pro@tramontina.com',
      //bcc: emailTramontina,
      subject:'Relatório diário SmartManager4.0',
      text: 'Relatório ' + String(configuracaoAtual.identificacao),
      attachments: [
        {
          content: attachment,
          filename: data + '.csv',
          type: 'application/csv',
          disposition: 'attachment'
        },
      ],
    }
  }
  else{
    email = {
      to: [configuracaoAtual.email, configuracaoAtual.email2, configuracaoAtual.email3],
      from: 'sistemainteligente.pro@tramontina.com',
      //bcc: emailTramontina,
      subject:'Relatório diário SmartManager4.0',
      text: 'Relatório ' + String(configuracaoAtual.identificacao) + '\nNenhum registro novo.'
    }
  }
  sgMail
    .send(email)
    .then(() => {
      console.log('Email enviado com sucesso')
      geraRelatorioTramontina('registros')
      try{
        timerRelatorioDiario.cancel()
      }catch(err){
        //console.log(err)
      }
      try{
        timerAdiaEmail.cancel()
      }catch(err){
        //console.log(err)
      }
      timerRelatorioDiario = schedule.scheduleJob('0 0 * * *', function(){
        geraRelatorio('registros')
        console.log('Gera relatorio e-mail 4')
      });
      Evento.updateMany({enviado:0}, {enviado:1}).then((eventos) => { //Atualiza multiplos eventos, indicando que já foram transmitidos
        console.log("Eventos atualizados")
      }).catch((err) => {
        console.log(err)
      })
      var date=new Date();
      Configuracao.updateOne({_id:'000000000000000000000001'}, {$set: {dataRelatorio: String(date.getDate()+'-'+(date.getMonth()+1)+'-'+date.getFullYear())}}).then(() => {
      }).catch((err) => {
        console.log("Erro ao sobrescrever configuracao: " + err)
      })
    })
    .catch((error) => {
      console.log('Erro ao enviar email: ' + error);
      //Ao ocorrer um erro, tenta enviar e-mail novamente alguns instantes depois
      adiaEnvioEmail()
    })
}

function adiaEnvioEmail(){
  today=new Date();
  var min = today.getMinutes()
  var hora = today.getHours()
  min = min+5
  if (min >= 60){
    hora = hora+1
    min = 0
    if (hora >= 24){
      hora = 0
    }
  }
  console.log("Nova tentativa de envio de relatorio: " + String(hora) + "h" + String(min))
  try{
    timerAdiaEmail.cancel()
  }catch(err){
    //console.log(err)
  }
  timerAdiaEmail = schedule.scheduleJob(String(min) + ' ' + String(hora) + ' * * *', function(){
    geraRelatorio('registros')
    console.log('Gera relatorio e-mail 5')
  })
}

wifi.init({
  iface: null // network interface, choose a random wifi interface if set to null
});

//editaConfiguracao(0,0,0,'email@email.com') //Carrega dados de configuração com valores iniciais

function editaConfiguracao(fechamento, cartao, desligamento, email, email2, email3, identificacao, encerrarSessao){
  Configuracao.findOne({_id:'000000000000000000000001'}).then((configuracao) => {
    if(!configuracao){
      const novaConfiguracao = {
        _id: ObjectId('000000000000000000000001'),
        fechamento: fechamento,
        cartao: cartao,
        desligamento: desligamento,
        encerrarSessao: encerrarSessao,
        email: email,
        email2: email2,
        email3: email3,
        identificacao: identificacao,
        dataRelatorio: '0'}
      new Configuracao(novaConfiguracao).save();
      configuracaoAtual = {
        fechamento: fechamento,
        cartao: cartao,
        desligamento: desligamento,
        encerrarSessao: encerrarSessao,
        email: email,
        email2: email2,
        email3: email3,
        identificacao: identificacao,
        dataRelatorio: '0'}
    }
    else{
      Configuracao.updateOne({_id:'000000000000000000000001'}, {$set: {fechamento:fechamento, cartao:cartao, desligamento: desligamento, email: email, email2: email2, email3: email3, identificacao: identificacao, encerrarSessao: encerrarSessao}}).then(() => {
        configuracaoAtual = {
        fechamento: fechamento,
        cartao: cartao,
        desligamento: desligamento,
        encerrarSessao: encerrarSessao,
        email: email,
        email2: email2,
        email3: email3,
        identificacao: identificacao}
      }).catch((err) => {
        console.log("Erro ao sobrescrever configuracao: " + err)
      })
    }
  }).catch((err) => {
    console.log('Erro ao acessar as configurações: ' + err)
  })
}

verificaFerramentasRemovidas(); //Verifica quantidade de ferramentas removidas ao inicializar o sistema

function verificaFerramentasRemovidas() {
  let iterable = [1,2,3,4,5,6,7,8]
  for(const value of iterable){ //Para executar ordenadamente, o laço for deve ser assim
    Ferramenta.find({gaveta:value, estado:0, desconhecido:0}).lean().populate("ferramentas").then((ferramentas) => { //Ferramentas removidas
      if(!ferramentas){
        console.log('Erro evento não encontrado')
      }
      else{
        ItensErrados[value-1] = ferramentas.length;
      }
    }).catch((err) => {
      console.log('Erro ao acessar banco de dados: ' + err)
    })
    Ferramenta.find({gaveta:value, desconhecido:1}).lean().populate("ferramentas").then((ferramentas) => { //Ferramentas estado desconhecido
      if(!ferramentas){
        console.log('Erro evento não encontrado')
      }
      else if (ferramentas.length > 0){ //Verifica se há alguma ferramenta com estado desconhecido
        ItensErrados[value-1] = 404;
      }
    }).catch((err) => {
      console.log('Erro ao acessar banco de dados: ' + err)
    })
  }
}

function abreCalculadora() {
  execFile(path.join('gnome-calculator'), (error, stdout, stderr) => {
  if (error) {
    console.error('error: ' + error);
    return;
  }
  })
}

/*function abreCalendario() {
  execFile(path.join('gnome-calendar'), (error, stdout, stderr) => {
  if (error) {
    console.error('error: ' + error);
    return;
  }
  })
}*/

function shutdownPC() {
  const child = spawn('shutdown', ['-h', 'now']);
}

function rebootPC() {
  const child = spawn('reboot');
}

function auxDesligamentoBateriaBaixa() {
  if (BateriaCarregando==0){
    console.log('Desligando o sistema por bateria baixa')
    shutdownPC()
  }
}

function novoEvento(ferramenta){
  var horario = getHorario();
  var data = getData();
  var usuarioatual = userLogged('showUserdata')
  const novoEvento = {
    referencia: ferramenta.referencia,
    descricao: ferramenta.descricao,
    gaveta: ferramenta.gaveta,
    posicao: ferramenta.posicao,
    dataRetirada: data,
    horaRetirada: horario,
    dataDevolucao: '-',
    horaDevolucao: '-',
    usuario: usuarioatual.login,
    setor: usuarioatual.setor,
    devolucao: 0,
    enviado: 0}
  new Evento(novoEvento).save();
}

function eventoDevolucao(ferramenta){
  var horario = getHorario();
  var data = getData();
  Evento.findOne({gaveta:ferramenta.gaveta, posicao:ferramenta.posicao, devolucao: 0}, null, {"sort": {"_id":-1}}).then((evento) => {
    if(!evento){
      console.log('Erro evento não encontrado')
    }
    else{
      const novoEvento = {
        referencia: evento.referencia,
        descricao: evento.descricao,
        gaveta: evento.gaveta,
        posicao: evento.posicao,
        dataRetirada: evento.dataRetirada,
        horaRetirada: evento.horaRetirada,
        dataDevolucao: data,
        horaDevolucao: horario,
        usuario: evento.usuario,
        setor: evento.setor,
        devolucao: 1,
        enviado: 0}
      Evento.deleteOne({_id:evento._id}).then(() => {
      }).catch((err) => {
        console.log("Erro ao deletar evento: " + err)
      })
      new Evento(novoEvento).save();
    }
  }).catch((err) => {
    console.log('Erro ao acessar banco de dados: ' + err)
  })
}

function getHorario(){
  var today=new Date();
  var h=today.getHours();
  var m=today.getMinutes();
  if (m<10) m="0" + m;
  return(h+':'+m);
}

function getData(){
  var today=new Date();
  var d=today.getDate();
  if (d<10) d="0"+d;
  var m=today.getMonth() + 1; //Mes inicia em 0
  if (m<10) m="0"+m;
  var y=today.getFullYear();
  return(d+'/'+m+'/'+y);
}

function convTempo(data){
  var auxString;
  var stringTempoDecorrido;
  stringTempoDecorrido = Math.floor(data/3600);
  auxString = Math.floor((data%3600)/60);
  if (auxString < 10){
    auxString = '0' + auxString;}
  stringTempoDecorrido = stringTempoDecorrido + ':' + auxString;
  auxString = Math.floor((data%3600)%60)
  if (auxString < 10){
    auxString = '0' + auxString;}
  stringTempoDecorrido = stringTempoDecorrido + ':' + auxString;
  return stringTempoDecorrido;
}
/*
function AtualizaDispositivo(){
  console.log("Iniciando atualização de software...")
  execFile('/home/tramontina/Downloads/update.sh', (error, stdout, stderr) => {
    if (error) {
      console.error('error: ' + error);
      return;
    }
    if (stderr) {
      console.error('stderr: ' + stderr);
      return;
    }
    if (stdout) {
      console.log("Software atualizado")
    }
  });
}*/

function validarEmail(email){
  var re = /\S+@\S+\.\S+/;
  return re.test(email);
}

/*process.on('uncaughtException', (error, origin) => {
  console.log('---- Uncaught exception ----')
  console.log(error)
  console.log('---- Exception origin ----')
  console.log(origin)
})

process.on('unhandledRejection', (reason, promise) => {
  console.log('---- Unhandled Rejection at ----')
  console.log(promise)
  console.log('---- Reason ----')
  console.log(reason)
})*/

//Funcções sockets
const method = function (io) {
    //Socket.IO
    
    var teste = io.on('connection', function (socket) {
        console.log('Usuario conectado');
        FuncSenha('resetPass', 0)

        resetTimer()

        function timerLogout(){
          console.log('auxGavetaAberta: ' + auxGavetaAberta)
          if (auxGavetaAberta==0){
            socket.emit('logoutUsuario')
          }
          else{
            resetTimer()
          }
        }
        
        function resetTimer(){
          try{
            auxTimer.close()
          }catch(err){
            //console.log(err)
          }
          //0-'1 min', 1-'3 min', 2-'5 min', 3-'10 min', 4-'Nunca'
          if (configuracaoAtual.encerrarSessao!=4){
            var temporizador
            if (configuracaoAtual.encerrarSessao==3) temporizador=600000
            else if (configuracaoAtual.encerrarSessao==2) temporizador=300000
            else if (configuracaoAtual.encerrarSessao==1) temporizador=180000
            else temporizador=60000
            auxTimer = setTimeout(function(){
              timerLogout();
            }, temporizador) //Temporizador em milisegundos
          }
        }

        socket.on('screenMousedown', function() {
          resetTimer()
        })

        socket.on('statusRedesNeurais', function() {
          socket.emit('redesNeuraisCarregadas', redesNeuraisCarregadas)
        })

        //Nível da bateria
        //socket.emit('atualizaBateria', ((cargaBateria-2)*20+'%'))
        verificaBateria()
        socket.on('nivelBateria', function() {
          if (abrindoGV==0 && fechandoGV==0)
            verificaBateria();
        })
        function verificaBateria() {
          execFile('/home/tramontina/Downloads/Interface/Bateria', (error, stdout, stderr) => {
            if (error) {
              console.error('error: ' + error);
              return;
            }
            if (stderr) {
              console.error('stderr: ' + stderr);
              return;
            }
            if (stdout) {
              //console.log(getHorario() + ' - Retorno bateria: ' + stdout)
              if(stdout[1]=='0' && stdout[2]=='0'){ //Bateria com o carregador desconectado
                BateriaCarregada = 0
                BateriaCarregando = 0
                if (arrayBateria.length==0){ //Inicializa com valores máximos, para evitar erros de leitura
                  cargaBateria = 5//stdout[0]
                  arrayBateria[0] = stdout[0]
                  arrayBateria[1] = stdout[0]
                  arrayBateria[2] = 5
                  arrayBateria[3] = 5
                  arrayBateria[4] = 5
                }
                else{
                  arrayBateria[4] = arrayBateria[3]
                  arrayBateria[3] = arrayBateria[2]
                  arrayBateria[2] = arrayBateria[1]
                  arrayBateria[1] = arrayBateria[0]
                  arrayBateria[0] = stdout[0]
                  if(arrayBateria[0] == arrayBateria[1] && arrayBateria[1] == arrayBateria[2] && arrayBateria[2] == arrayBateria[3] 
                  && arrayBateria[3] == arrayBateria[4] && cargaBateria >= arrayBateria[0])
                    cargaBateria = arrayBateria[0];
                    if (cargaBateria>5) cargaBateria=5;
                }
                //console.log('Array bateria: ' + arrayBateria)
                //console.log('Carga bateria: ' + cargaBateria)
                //console.log('Nivel bateria: ' + ((cargaBateria-2)*20+'%'));
                socket.emit('atualizaBateria', ((cargaBateria)*20+'%'))
                //console.log('Nivel bateria: ' + ((cargaBateria)*20+'%'))

                if (cargaBateria==0){ //Nivel de bateria minimo, desliga pc após temporizacao
                                      //1 é o nivel minimo, 0 indica que a placa está desconectada
                  console.log('Inicia timer de 5 minutos para desligar o sistema por bateria baixa')
                  setTimeout(() => {  //Desliga sistema após 5 minutos
                    //shutdownPC()
                    auxDesligamentoBateriaBaixa()
                  }, 300000);
                }
              }
              else if (stdout[1]=='1'){ //Bateria carregada
                BateriaCarregada = 1
                BateriaCarregando = 0
                arrayBateria = []
                socket.emit('atualizaBateria', 'carregada')
              }
              else if (stdout[2]=='1'){ //Bateria carregando
                BateriaCarregada = 0
                BateriaCarregando = 1
                arrayBateria = []
                socket.emit('atualizaBateria', 'carregando')
              }
            }
          });
        }

        //Status Wifi
        socket.emit('atualizaWifi', redeWifiOk, wifiLevel)
        socket.on('statusWifi', function() {
          verificaConexaoWifi();
        })
        function verificaConexaoWifi(){
          wifi.getCurrentConnections((error, network) => {
            if (error) {
              console.log(error);
            } else {
              if (network.length > 0){
                //console.log("Conectado à rede: " + network[0].ssid + " " + network[0].signal_level);
                redeWifiOk = 1
                wifiLevel = network[0].signal_level
                socket.emit('atualizaWifi', 1, network[0].signal_level)
              }
              else{
                //console.log("Wifi não conectado");
                redeWifiOk = 0
                wifiLevel = 0
                socket.emit('atualizaWifi', 0, 0)
              }
            }
            //socket.emit('atualizaWifi')
          });
        }
     
        //Tela login
        socket.on('insertKey', function(data) {
           senha=FuncSenha('insertKey', data)
           socket.emit('SenhaAtual', senha)
        });
        socket.on('removeKey', function(data) {
        	 senha=FuncSenha('removeKey', data)
           socket.emit('SenhaAtual', senha)
        });

        //Tela main
        socket.on('estadoGavetas', function() {
          estadoGavetas()
        });
        socket.on('estadoFerramentas', function() {
          //console.log('Itens errados: ' + ItensErrados)
          socket.emit('estadoFerramentas', ItensErrados);
        });
        socket.on('abreGaveta', function(data) {
          if (abrindoGV==0 && fechandoGV==0 && bloqueiaAbertura==0){
            auxFGVSemRedeNeural = 0
            abrindoGV = 1
            bloqueiaAbertura = 1
            gavetaEmMovimento = data
            socket.emit('bloqueiaInteracaoUsuario')
            abreGaveta(data)
            //carregaRedeNeural(data)
            //iniciaCameras(data)
            //console.log("PID: " + childCam1.pid)
            //console.log("PID: " + childCam2.pid)
            try {
              process.kill(childCam1.pid)
            } catch (error){
              console.log('Kill cam1 - Erro encontrado: ' + error)
            }
            try {
              process.kill(childCam2.pid)
            } catch (error){
              console.log('Kill cam2 - Erro encontrado: ' + error)
            }
            iniciaCam1(data)
            iniciaCam2(data)
          }
        });
        socket.on('fechaGaveta', function(data) {
          if (abrindoGV==0 && fechandoGV==0){
            //auxFGVSemRedeNeural = 0
            fechandoGV = 1
            gavetaEmMovimento = data
            socket.emit('bloqueiaInteracaoUsuario')
            fechaGaveta(data)
          }
        });
        socket.on('abreCalculadora', function() {
           abreCalculadora()
        });
        /*socket.on('abreCalendario', function() {
          abreCalendario()
        });*/
        socket.on('shutdown', function() {
           shutdownPC()
        });
        socket.on('reboot', function() {
           rebootPC()
        });
        socket.on('connectWifi', function(ssid, password) {
          connectWifi(ssid, password)
        });
        socket.on('disconnectWifi', function() {
          disconnectWifi()
        });

        //Gera e envia relatório de checkout
        socket.on('emiteRelatorioCheckout', function() {
          geraRelatorioCheckout()
        });

        function connectWifi(ssid, password) {
          console.log("Conectando à rede...")
          wifi.connect({ ssid: ssid, password: password }, error => {
            var strError = String(error)
            if (!error){
              console.log('Conectado');
              socket.emit('resultadoConexao', 1, 0)
            }
            else if (strError.indexOf("Error: Connection activation failed")>-1){ //Verifica se o erro contém string que indica credenciais inválidas
              console.log("Erro ao conectar: " + error)
              socket.emit('resultadoConexao', 0, 0) //0->Senha inválida / 1->Outros erros
              //disconnectWifi()
            }
            else{
              console.log("Erro ao conectar: " + error)
              socket.emit('resultadoConexao', 0, 1) //0->Senha inválida / 1->Outros erros
              //disconnectWifi()
            }
            verificaConexaoWifi();
          });
        }
        
        function disconnectWifi(){
          console.log('Desconectando...')
          wifi.disconnect(error => {
            if (error) {
              console.log(error);
            } else {
              console.log('Desconectado');
              setTimeout(() => {socket.emit('Desconectado'); }, 500)
            }
          });
        }
        
        socket.on('GvTrancadaAGV', function() {
          if (fechandoGV==0){
            abreGaveta(gavetaEmMovimento)
          }
          else{
            fechandoGV = 0
            abrindoGV = 1
            bloqueiaAbertura = 1
            //carregaRedeNeural(gavetaEmMovimento)
            try {
              process.kill(childCam1.pid)
            } catch (error){
              console.log('Kill cam1 - Erro encontrado: ' + error)
            }
            try {
              process.kill(childCam2.pid)
            } catch (error){
              console.log('Kill cam2 - Erro encontrado: ' + error)
            }
            iniciaCam1(gavetaEmMovimento)
            iniciaCam2(gavetaEmMovimento)            
            abreGaveta(gavetaEmMovimento)
          }
        });
        socket.on('GvTrancadaFGV', function() {
          auxFGVSemRedeNeural = 1
          try {
            process.kill(childCam1.pid)
          } catch (error){
            console.log('Kill cam1 - Erro encontrado: ' + error)
          }
          try {
            process.kill(childCam2.pid)
          } catch (error){
            console.log('Kill cam2 - Erro encontrado: ' + error)
          }
          try {
            process.kill(childVerificacao.pid)
          } catch (error){
            console.log('Kill rede neural - Erro encontrado: ' + error)
          }
          socket.emit('showGvTrancadaFechando', gavetaEmMovimento)
          ItensErrados[gavetaEmMovimento-1] = 404
          Ferramenta.find({gaveta:gavetaEmMovimento}).then((ferramentas) => {
            if(!ferramentas){
              console.log('Erro ferramentas não encontradas')
            }
            else{
              for(var i=0; i<(ferramentas.length); i++){
                ferramentas[i].desconhecido=1;
                ferramentas[i].save(); //Armazena dados atualizados no banco
              }
            }
          }).catch((err) => {
            console.log('Erro ao acessar banco de dados: ' + err)
          })
          fechandoGV = 1
          fechaGaveta(gavetaEmMovimento)
        });

        function iniciaCam1(data){
          //console.log('iniciando camera da gaveta ' + data)
          childCam1 = spawn('/home/tramontina/Downloads/Interface/GetImages_GV' + data + '_c1', [], {detached: false});
          //console.log('PID inicial: ' + childCam1.pid)
          childCam1.on('exit', (code) => {
            console.log("Cam1 finalizada...")
          });

          childCam1.stdout.on('data', (data) => {
              //console.log('std: '+data);
          });

          childCam1.stderr.on('data', (data) => {
              //console.log('stderr: ' + data);
          });
        }

        function iniciaCam2(data){
          //console.log('iniciando camera da gaveta ' + data)
          childCam2 = spawn('/home/tramontina/Downloads/Interface/GetImages_GV' + data + '_c2', [], {detached: false});
          
          childCam2.on('exit', (code) => {
            console.log("Cam2 finalizada...")
          });

          childCam2.stdout.on('data', (data) => {
              //console.log('std: '+data);
          });

          childCam2.stderr.on('data', (data) => {
              //console.log('stderr: ' + data);
          });
        }

        /*function carregaRedeNeural(gaveta) {
          //console.log('Carregando rede neural da gaveta ' + gaveta)
          childRedeNeural = spawn('/home/tramontina/Downloads/Gavetas/RedeNeural_GV' + gaveta, [], {detached: false});
          
          childRedeNeural.on('exit', (code) => {
              //impedeFGV1 = 0;
              //socket.emit('show_view', 1);
              //console.log('Child process exited with code ' + code);
              //var resultado = ItensErrados[gaveta-1];
              if (auxFGVSemRedeNeural==0){
                socket.emit('result_GV', ItensErrados[gaveta-1]);}
              bloqueiaAbertura = 0;
              //for (var i=0; i<(data.length-1); i++){
              //  console.log(i + ': ' + data[i])
              //}
              //enviaResultadoRedeNeural(ItensErrados[gaveta-1]);
          });
          childRedeNeural.stdout.on('data', (data) => {
              ItensErrados[gaveta-1] = 0;
              //console.log('Resultado da rede: ' + data)
              for(var i=0; i<(data.length-1); i++){
                Ferramenta.findOne({gaveta:gaveta, posicao:i+1}).then((ferramenta) => {
                  if(!ferramenta){
                    console.log('Erro ferramenta não encontrada')
                  }
                  else{
                    //Verifica quais item estão incorretos
                    //48=0(errado), 49=1(correto), conforme tabela ASCII
                    if (data[ferramenta.posicao-1]==49 && ferramenta.estado!=1) { //Ferramenta devolvida
                      console.log('' + ferramenta.descricao + ' devolvida');
                      ferramenta.estado=1;
                      eventoDevolucao(ferramenta);
                    }
                    else if (data[ferramenta.posicao-1]==48 && ferramenta.estado!=0) { //Ferramenta retirada
                      ItensErrados[gaveta-1] = ItensErrados[gaveta-1]+1;
                      console.log('' + ferramenta.descricao + ' retirada');
                      ferramenta.estado=0;
                      novoEvento(ferramenta);
                      //Grava histórico de retirada da ferramenta
                    }
                    else if (data[ferramenta.posicao-1]==48){
                      ItensErrados[gaveta-1] = ItensErrados[gaveta-1]+1;
                    }
                  }
                  ferramenta.save(); //Armazena dados atualizados no banco
                }).catch((err) => {
                  console.log('Erro ao acessar banco de dados ferramenta: ' + err)
                })
              }
          });
          childRedeNeural.stderr.on('data', (data) => {
              //console.log('stderr: ' + data);
          });
        }*/

        function carregaRedeNeural(gaveta) {
          //console.log('Carregando rede neural da gaveta ' + gaveta)
          var falhaCameras = 0;
          childVerificacao = spawn('/home/tramontina/Downloads/Interface/Verificacao', ['GV'+gaveta], {detached: false});
          
          childVerificacao.on('exit', (code) => {
              //impedeFGV1 = 0;
              //socket.emit('show_view', 1);
              //console.log('Child process exited with code ' + code);
              //var resultado = ItensErrados[gaveta-1];
              if (auxFGVSemRedeNeural==0 && falhaCameras==0){
                socket.emit('result_GV', ItensErrados[gaveta-1]);}
              bloqueiaAbertura = 0;
              //for (var i=0; i<(data.length-1); i++){
              //  console.log(i + ': ' + data[i])
              //}
              //enviaResultadoRedeNeural(ItensErrados[gaveta-1]);
          });
          childVerificacao.stdout.on('data', (data) => {
              console.log('Resultado da rede: ' + data)
              if(data=='FCAM1'){ 
                falhaCameras = 1;
                socket.emit('falhaComunicacaoCamera', 1)
                console.log('Falha na camera 1...')}
              else if(data=='FCAM2'){
                falhaCameras = 1;
                socket.emit('falhaComunicacaoCamera', 2)
                console.log('Falha na camera 2...')}
              else if(data=='FCAM12'){
                falhaCameras = 1;
                socket.emit('falhaComunicacaoCamera', 12) 
                console.log('Falha nas cameras 1 e 2...')
                }
              else{
                ItensErrados[gaveta-1] = 0;
                for(var i=0; i<(data.length); i++){
                  Ferramenta.findOne({gaveta:gaveta, posicao:i+1}).then((ferramenta) => {
                    if(!ferramenta){
                      console.log('Erro ferramenta não encontrada')
                    }
                    else{
                      //Verifica quais item estão incorretos
                      //48=0(errado), 49=1(correto), conforme tabela ASCII
                      ferramenta.desconhecido=0;
                      if (data[ferramenta.posicao-1]==49 && ferramenta.estado==0) { //Ferramenta devolvida
                        console.log('' + ferramenta.descricao + ' devolvida');
                        ferramenta.estado=1;
                        eventoDevolucao(ferramenta);
                      }
                      else if (data[ferramenta.posicao-1]==48 && ferramenta.estado==1) { //Ferramenta retirada
                        ItensErrados[gaveta-1] = ItensErrados[gaveta-1]+1;
                        console.log('' + ferramenta.descricao + ' retirada');
                        ferramenta.estado=0;
                        novoEvento(ferramenta);
                        //Grava histórico de retirada da ferramenta
                      }
                      else if (data[ferramenta.posicao-1]==48){
                        ItensErrados[gaveta-1] = ItensErrados[gaveta-1]+1;
                      }
                    }
                    ferramenta.save(); //Armazena dados atualizados no banco
                  }).catch((err) => {
                    console.log('Erro ao acessar banco de dados ferramenta: ' + err)
                  })
                }
              }
          });
          childVerificacao.stderr.on('data', (data) => {
              //console.log('stderr: ' + data);
          });
        }

        //Funções auxiliares
        function abreGaveta(data) {
            //if(data==1 && impedeFGV1==0){
            //  socket.emit('show_view', 0);
            //  impedeFGV1 = 1;
            //  carregaRedeNeural();
            //}
            //Inicia abertura insere dado de localização das ferramentas desconhecida
            auxFGVSemRedeNeural = 0
            ItensErrados[data-1] = 404
            Ferramenta.find({gaveta:data}).then((ferramentas) => {
              if(!ferramentas){
                console.log('Erro ferramentas não encontradas')
              }
              else{
                for(var i=0; i<(ferramentas.length); i++){
                  ferramentas[i].desconhecido=1;
                  ferramentas[i].save(); //Armazena dados atualizados no banco
                }
              }
            }).catch((err) => {
              console.log('Erro ao acessar banco de dados: ' + err)
            })
            //
            execFile('/home/tramontina/Downloads/Interface/Gavetas', ['AGV'+data], (error, stdout, stderr) => {
              if (error) {
                console.error('error: ' + error);
                return;
              }
              if (stderr) {
                console.error('stderr: ' + stderr);
                return;
              }
              console.log('stdout: ' + stdout);
              if (stdout[2]=='A'){
                abrindoGV = 0
                console.log("Gaveta aberta")
                auxGavetaAberta = 1
                resetTimer()
                socket.emit('gavetaAberta', data);
              }
              else if (stdout=='GVT'){
                //abrindoGV = 0
                //bloqueiaAbertura = 0
                console.log("Gaveta trancada")
                auxGavetaAberta = 1
                resetTimer()
                socket.emit('gavetaTrancada', data);}
            });
        }

        function fechaGaveta(data) {
            abrindoGV = 0
            //bloqueiaAbertura = 0
            execFile('/home/tramontina/Downloads/Interface/Gavetas', ['FGV'+data], (error, stdout, stderr) => {
              if (error) {
                console.error('error: ' + error);
                return;
              }
              if (stderr) {
                console.error('stderr: ' + stderr);
                return;
              }
              console.log('stdout: ' + stdout);
              if (stdout[2]=='F'){
                fechandoGV = 0
                console.log("Gaveta fechada")
                auxGavetaAberta = 0
                resetTimer()
                socket.emit('gavetaFechada', data);
                if (auxFGVSemRedeNeural==0){
                  try {
                    process.kill(childVerificacao.pid)
                  } catch (error){
                    console.log('Kill rede neural - Erro encontrado: ' + error)
                  }
                  carregaRedeNeural(data)
                }
                else{
                  bloqueiaAbertura = 0
                  ItensErrados[data-1] = 404
                  socket.emit('result_GV', ItensErrados[data-1])
                  Ferramenta.find({gaveta:gavetaEmMovimento}).then((ferramentas) => {
                    if(!ferramentas){
                      console.log('Erro ferramentas não encontradas')
                    }
                    else{
                      for(var i=0; i<(ferramentas.length); i++){
                        ferramentas[i].desconhecido=1;
                        console.log("Ferramenta " + i + ": " + ferramentas[i].descricao)
                        ferramentas[i].save(); //Armazena dados atualizados no banco
                      }
                    }
                  }).catch((err) => {
                    console.log('Erro ao acessar banco de dados: ' + err)
                  })
                  auxFGVSemRedeNeural = 0
                }
              }
              else if (stdout=='GVT'){
                //fechandoGV = 0
                auxFGVSemRedeNeural = 1
                try {
                  process.kill(childCam1.pid)
                } catch (error){
                  console.log('Kill cam1 - Erro encontrado: ' + error)
                }
                try {
                  process.kill(childCam2.pid)
                } catch (error){
                  console.log('Kill cam2 - Erro encontrado: ' + error)
                }
                try {
                  process.kill(childVerificacao.pid)
                } catch (error){
                  console.log('Kill rede neural - Erro encontrado: ' + error)
                }
                console.log("Gaveta trancada")
                auxGavetaAberta = 1
                resetTimer()
                socket.emit('gavetaTrancada', data);}
            })
        }

        function estadoGavetas() {
          execFile('/home/tramontina/Downloads/Interface/Gavetas', ['GAVETAS'], (error, stdout, stderr) => {
            if (error) {
              console.error('error: ' + error);
              return;
            }
            if (stderr) {
              console.error('stderr: ' + stderr);
              return;
            }
            console.log('stdout: ' + stdout);
            socket.emit('estadoGavetas', stdout);
            if (stdout[0]=='A'||stdout[1]=='A'||stdout[2]=='A'||stdout[3]=='A'||stdout[4]=='A'||
            stdout[5]=='A'||stdout[6]=='A'||stdout[7]=='A'){
              console.log("Há gaveta aberta")
              auxGavetaAberta=1
            }
            else{
              console.log("Todas as gavetas fechadas")
              auxGavetaAberta=0
            }
          });
        }

        function enviaEstadoGavetas(status) {
          socket.emit('estadoGavetas', status);
        }
        
        //Testes gaveta específica
        socket.on('testegavetaespecifica', function(gaveta, repeticoes) {
          console.log('Gaveta: ' + gaveta + '\nRepeticoes: ' + repeticoes);
          numRepeticoesGVEspecifica = repeticoes;
          pausaTestesGavetas = 0;
          paraTestesGavetas = 0;
          testaGavetaEspecifica(gaveta);
          console.log('Iniciando teste')
        });

        socket.on('pausaTesteGVEspecifica', function() {
          pausaTestesGavetas = 1;
          console.log('Pausando teste ' + pausaTestesGavetas)
        });

        socket.on('stopTesteGVEspecifica', function() {
          paraTestesGavetas = 1;
          contRepeticoesGVEspecifica = 0;
          if (testesGavetasAndamento==0){
            contRepeticoesGVEspecifica = 0;
            numRepeticoesGVEspecifica = 0;
            pausaTestesGavetas = 0;
            testesGavetasAndamento = 0;
            paraTestesGavetas = 0;
            socket.emit('testeParadoGVEspecifica')
          }
          console.log('Parando teste ' + pausaTestesGavetas)
        });

        socket.on('estadoTestesGVEspecifica', function() {
          socket.emit('estadoTestesGVEspecifica', testesGavetasAndamento, contRepeticoesGVEspecifica);
        });

        function testaGavetaEspecifica(gaveta){
          testesGavetasAndamento = 1;
          console.log('Cont. repeticoes: ' + contRepeticoesGVEspecifica)
          socket.emit('atualizaContadorTestesGVEspecifica', contRepeticoesGVEspecifica)
          if (contRepeticoesGVEspecifica >= numRepeticoesGVEspecifica){
            var data = getData();
            var stringTempGVEspecifica = convTempo(segDecorridoGVEspecifica);
            if (contRepeticoesGVEspecifica > 0){
              const novoTeste = {
                data: data,
                gaveta: gaveta,
                tempo: stringTempGVEspecifica,
                contagemCiclos: contRepeticoesGVEspecifica,
                tipo: 0,
                finalizado: 1}
              new DadosTeste(novoTeste).save();
            }
            contRepeticoesGVEspecifica = 0;
            numRepeticoesGVEspecifica = 0;
            pausaTestesGavetas = 0;
            testesGavetasAndamento = 0;
            paraTestesGavetas = 0;
            socket.emit('testeFinalizadoGVEspecifica')}
          else if (pausaTestesGavetas == 0 && paraTestesGavetas == 0){
            AGVTestesEspecifico(gaveta)}
          else if (paraTestesGavetas == 1){
            testesGavetasAndamento = 0;
            pausaTestesGavetas = 0;
            paraTestesGavetas = 0;
            socket.emit('testeParadoGVEspecifica')}
          else if (pausaTestesGavetas == 1){
            testesGavetasAndamento = 0;
            pausaTestesGavetas = 0;
            socket.emit('testePausadoGVEspecifica')}
        }

        function AGVTestesEspecifico(gaveta){
          execFile('/home/tramontina/Downloads/Interface/Gavetas', ['AGV'+gaveta], (error, stdout, stderr) => {
            if (error) {
              console.error('error: ' + error);}
            if (stderr) {
              console.error('stderr: ' + stderr);}
            console.log('stdout: ' + stdout);
            if (stdout[2]=='A'){
              console.log("Gaveta aberta")
              FGVTestesEspecifico(gaveta)}
            else if (stdout=='GVT'){
              var data = getData();
              var stringTempGVEspecifica = convTempo(segDecorridoGVEspecifica);
              const novoTeste = {
                data: data,
                gaveta: gaveta,
                tempo: stringTempGVEspecifica,
                contagemCiclos: contRepeticoesGVEspecifica,
                tipo: 0,
                finalizado: 0}
              new DadosTeste(novoTeste).save();
              pausaTestesGavetas = 0;
              testesGavetasAndamento = 0;
              paraTestesGavetas = 0;
              socket.emit('testeFalhouGVEspecifica')
              console.log("Gaveta trancada")}
          })
        }

        function FGVTestesEspecifico(gaveta){
          execFile('/home/tramontina/Downloads/Interface/Gavetas', ['FGV'+gaveta], (error, stdout, stderr) => {
            if (error) {
              console.error('error: ' + error);}
            if (stderr) {
              console.error('stderr: ' + stderr);}
            console.log('stdout: ' + stdout);
            if (stdout[2]=='F'){
              console.log("Gaveta fechada")
              contRepeticoesGVEspecifica = contRepeticoesGVEspecifica + 1
              testaGavetaEspecifica(gaveta)}
            else if (stdout=='GVT'){
              var data = getData();
              var stringTempGVEspecifica = convTempo(segDecorridoGVEspecifica);
              const novoTeste = {
                data: data,
                gaveta: gaveta,
                tempo: stringTempGVEspecifica,
                contagemCiclos: contRepeticoesGVEspecifica,
                tipo: 0,
                finalizado: 0}
              new DadosTeste(novoTeste).save();
              pausaTestesGavetas = 0;
              testesGavetasAndamento = 0;
              paraTestesGavetas = 0;
              socket.emit('testeFalhouGVEspecifica')
              console.log("Gaveta trancada")}
          })
        }

        //Testes todas as gavetas
        socket.on('testetodasgavetas', function(repeticoes) {
          console.log('Repeticoes: ' + repeticoes);
          numRepeticoesTodasGVs = repeticoes;
          pausaTestesGavetas = 0;
          paraTestesGavetas = 0;
          testaTodasGavetas();
          console.log('Iniciando teste de todas as gavetas...')
        });

        socket.on('pausaTesteTodasGVs', function() {
          pausaTestesGavetas = 1;
          console.log('Pausando teste ' + pausaTestesGavetas)
        });

        socket.on('stopTesteTodasGVs', function() {
          paraTestesGavetas = 1;
          contRepeticoesTodasGVs = 0;
          if (testesGavetasAndamento==0){
            contRepeticoesTodasGVs = 0;
            numRepeticoesTodasGVs = 0;
            pausaTestesGavetas = 0;
            testesGavetasAndamento = 0;
            paraTestesGavetas = 0;
            gavetaEmTeste = 1;
            socket.emit('testeParadoTodasGVs')
          }
          console.log('Parando teste ' + pausaTestesGavetas)
        });

        socket.on('estadoTestesTodasGVs', function() {
          socket.emit('estadoTestesTodasGVs', testesGavetasAndamento, contRepeticoesTodasGVs);
        });

        function testaTodasGavetas(){
          console.log("aux...")
          testesGavetasAndamento = 1;
          console.log('Cont. repeticoes: ' + contRepeticoesTodasGVs)
          socket.emit('atualizaContadorTestesTodasGVs', contRepeticoesTodasGVs)
          if (contRepeticoesTodasGVs >= numRepeticoesTodasGVs){
            var data = getData();
            var stringTempTodasGVs = convTempo(segDecorridoTodasGVs);
            if (contRepeticoesTodasGVs > 0){
              const novoTeste = {
                data: data,
                gaveta: 0,
                tempo: stringTempTodasGVs,
                contagemCiclos: contRepeticoesTodasGVs,
                tipo: 1,
                finalizado: 1}
              new DadosTeste(novoTeste).save();
            }
            contRepeticoesTodasGVs = 0;
            numRepeticoesTodasGVs = 0;
            gavetaEmTeste = 1;
            pausaTestesGavetas = 0;
            testesGavetasAndamento = 0;
            paraTestesGavetas = 0;
            socket.emit('testeFinalizadoTodasGVs')}
          else if (pausaTestesGavetas == 0 && paraTestesGavetas == 0){
            AGVTestesTodasGavetas()}
          else if (paraTestesGavetas == 1){
            testesGavetasAndamento = 0;
            pausaTestesGavetas = 0;
            paraTestesGavetas = 0;
            gavetaEmTeste = 1;
            socket.emit('testeParadoTodasGVs')}
          else if (pausaTestesGavetas == 1){
            testesGavetasAndamento = 0;
            pausaTestesGavetas = 0;
            socket.emit('testePausadoTodasGVs')}
        }

        function AGVTestesTodasGavetas(){
          execFile('/home/tramontina/Downloads/Interface/Gavetas', ['AGV'+gavetaEmTeste], (error, stdout, stderr) => {
            if (error) {
              console.error('error: ' + error);}
            if (stderr) {
              console.error('stderr: ' + stderr);}
            console.log('stdout: ' + stdout);
            if (stdout[2]=='A'){
              console.log("Gaveta aberta")
              FGVTestesTodasGavetas()}
            else if (stdout=='GVT'){
              var data = getData();
              var stringTempTodasGVs = convTempo(segDecorridoTodasGVs);
              const novoTeste = {
                data: data,
                gaveta: 0,
                tempo: stringTempTodasGVs,
                contagemCiclos: contRepeticoesTodasGVs,
                tipo: 1,
                finalizado: 0}
              new DadosTeste(novoTeste).save();
              pausaTestesGavetas = 0;
              testesGavetasAndamento = 0;
              paraTestesGavetas = 0;
              gavetaEmTeste = 1;
              socket.emit('testeFalhouTodasGVs')
              console.log("Gaveta trancada")}
          })
        }

        function FGVTestesTodasGavetas(){
          execFile('/home/tramontina/Downloads/Interface/Gavetas', ['FGV'+gavetaEmTeste], (error, stdout, stderr) => {
            if (error) {
              console.error('error: ' + error);}
            if (stderr) {
              console.error('stderr: ' + stderr);}
            console.log('stdout: ' + stdout);
            if (stdout[2]=='F'){
              console.log("Gaveta fechada")
              if (gavetaEmTeste < 8){
                gavetaEmTeste = gavetaEmTeste + 1
              }
              else{
                contRepeticoesTodasGVs = contRepeticoesTodasGVs + 1
                gavetaEmTeste = 1
              }
              testaTodasGavetas()}
            else if (stdout=='GVT'){
              var data = getData();
              var stringTempTodasGVs = convTempo(segDecorridoTodasGVs);
              const novoTeste = {
                data: data,
                gaveta: 0,
                tempo: stringTempTodasGVs,
                contagemCiclos: contRepeticoesTodasGVs,
                tipo: 1,
                finalizado: 0}
              new DadosTeste(novoTeste).save();
              pausaTestesGavetas = 0;
              testesGavetasAndamento = 0;
              paraTestesGavetas = 0;
              gavetaEmTeste = 1;
              socket.emit('testeFalhouTodasGVs')
              console.log("Gaveta trancada")}
          })
        }

        socket.on('auxBuscarAtualizacoes', function() {
          if (redeWifiOk==1){
            BuscarAtualizacoes();
          }
          else{
            socket.emit('resultadoBuscaAtualizacao', 0)
          }
        })

        socket.on('auxAtualizaDispositivo', function() {
          if (redeWifiOk==1){
            AtualizaDispositivo();
            socket.emit('resultadoAtualizacao', 1)
          }
          else{
            socket.emit('resultadoAtualizacao', 0)
          }
        })

        function BuscarAtualizacoes(){
          console.log("Verificando versão mais recente...")
          fs.chmodSync('searchVersion.sh', '777')
          childSearchVersion = spawn('/home/tramontina/Downloads/Interface/searchVersion.sh', [], {detached: true});
          childSearchVersion.on('exit', (code) => {
              var path = require('path');
              var filename = require.resolve('../latestVersion.js');
              delete require.cache[filename];
              var ultimaVersao = require("../latestVersion");
              console.log('Versao mais recente: ' + ultimaVersao)
              socket.emit('resultadoBuscaAtualizacao', ultimaVersao)
          });
          childSearchVersion.stdout.on('data', (data) => {
                console.log(data)
          });
          childSearchVersion.stderr.on('data', (data) => {
              console.log('stderr: ' + data);
          });
        }

        function AtualizaDispositivo(){
          console.log("Iniciando atualização de software...")
          fs.chmodSync('update.sh', '777')
          childAtualizaSoftware = spawn('/home/tramontina/Downloads/Interface/update.sh', [], {detached: true});
          childAtualizaSoftware.on('exit', (code) => {
              console.log('Software atualizado')
              rebootPC()
          });
          childAtualizaSoftware.stdout.on('data', (data) => {
                console.log(data)
          });
          childAtualizaSoftware.stderr.on('data', (data) => {
              console.log('stderr: ' + data);
          });
        }

        socket.on('atualizaTempTestesGVEspecifica', function(data) {
          segDecorridoGVEspecifica = data;
        })

        socket.on('atualizaTempTestesTodasGVs', function(data) {
          segDecorridoTodasGVs = data;
        })

        socket.on('salvaConfiguracoes', function(confFechamento, confCartao, confDesligamento, confEmail, confEmail2, confEmail3, confIdentificacao, confEncerrarSessao){
          console.log('Salvando configuração...')
          console.log('Ident: ' + confIdentificacao)
          //setTimeout(() => {resetTimer(); }, 500)
          editaConfiguracao(confFechamento, confCartao, confDesligamento, confEmail, confEmail2, confEmail3, confIdentificacao, confEncerrarSessao)
        })

        socket.on('excluiRegistros', function(data) {
          if (data==0){
            console.log("Exclui resgistros com ferramentas devolvidas")
            Evento.deleteMany({devolucao:1}).then(() => { //body.id porque esse parâmetro virá do formulário oculto com o id criado no categorias.handlebars
              console.log("Histórico deletado com sucesso!")
            }).catch((err) => {
              console.log("Houve um erro ao deletar o histórico: " + err)
            })
          }
          else if (data==1){
            console.log("Exclui todos os resgistros")
            Evento.deleteMany().then(() => { //body.id porque esse parâmetro virá do formulário oculto com o id criado no categorias.handlebars
              console.log("Histórico deletado com sucesso!")
            }).catch((err) => {
              console.log("Houve um erro ao deletar o histórico: " + err)
            })
          }
        })

        //Retorna versão atual do software
        socket.on('verificaVersaoSoftware', function(){
          socket.emit('versaoSoftware', versaoSoftware)
        })

        //Configura acesso remoto
        socket.on('iniciaNgrokHTTP', function(){ //Executa ngrok para acesso de interface gráfica
          if (redeWifiOk==1)
            ngrokHTTPConnect()
          else
            socket.emit('falhaConexao')
        })

        socket.on('iniciaNgrokSSH', function(){ //Executa ngrok para acesso pro SSH
          if (redeWifiOk==1)
            ngrokSSHConnect()
          else
            socket.emit('falhaConexao')
        })

        socket.on('encerraNgrok', function(){ //Executa ngrok para acesso pro SSH
          encerraNgrok()
        })

		function ngrokHTTPConnect(){
          console.log("Conectando ao ngrok HTTP...")
          try{ //Verifica numero de série do organizador
            ngrok.connect({region:'sa', proto: 'http', addr:8081, authtoken:'20BAgjvFYCzY3LJwy9VMch2YkrT_7PqDGu6Cgikww4TZudh2u'}).then(url =>{
              console.log('Url da conexao: ' + url)
              socket.emit('HTTPConectado', url)
            }).catch(err =>{
              console.log("Erro ao conectar ao ngrok: " + err)
              socket.emit('falhaConexao')
              ngrok.kill();
            })
          }catch (error){
            console.log("Erro ao conectar ao ngrok: " + error)
            socket.emit('falhaConexao')
            ngrok.kill();
          }
        }

        function ngrokSSHConnect(){
          console.log("Conectando ao ngrok SSH...")
          try{ //Verifica numero de série do organizador
            ngrok.connect({region:'sa', proto: 'tcp', addr:22, authtoken:'20BAgjvFYCzY3LJwy9VMch2YkrT_7PqDGu6Cgikww4TZudh2u'}).then(url =>{
              console.log('Url da conexao: ' + url)
              socket.emit('SSHConectado', url)
            }).catch(err =>{
              console.log("Erro ao conectar ao ngrok: " + err)
              socket.emit('falhaConexao')
              ngrok.kill();
            })
          }catch (error){
            console.log("Erro ao conectar ao ngrok: " + error)
            socket.emit('falhaConexao')
            ngrok.kill();
          }
        }

        function encerraNgrok(){
          console.log("Encerrando ngrok...")
          ngrok.disconnect();
        }

        socket.on('alteraDataHora', function(data){
          console.log(data)
          setDateTime(data)
        })

        socket.on('getSerialNumber', function(){
          socket.emit('setSerialNumber', serialOrganizador)
        })

        //Verifica IP
        socket.on('verificaIP', function(){
          exec("ifconfig wlan0|awk '/inet / {print $2}'", (error, stdout, stderr)=>{
            if (stdout){
              console.log('Endereço IP: ' + stdout);
              socket.emit('retornaIP', stdout)
            }
          })
        })

        //Restaura sistema original de fábrica

        socket.on('restauraSistema', function(){
          /*if (restoreDatabaseOrganizador()){
            console.log('Restauração concluida')
          }
          else{
            console.log('Erro ao restaurar o sistema')
          }*/
          auxRestauraOrganizador()
        })

        async function auxRestauraOrganizador(){
          var x = await restoreDatabaseOrganizador();
          console.log("Database 'organizadorinteligente' restaurado com sucesso: " + x)
          //x = await restoreDatabaseAdmin();
          //console.log("Database 'admin' restaurado com sucesso: " + x)
          x = await restoreInterface();
          console.log("Interface restaurada com sucesso: " + x)
          rebootPC()
        }

        //Restaura backup do database e arquivos da interface
        function restoreDatabaseOrganizador() {
          return new Promise(resolve => {
            var childRestore = child_restoreSystem.exec(`mongorestore --username "administrador" --password "EapIk9oc" --authenticationDatabase admin --db organizadorinteligente --drop dboriginal/organizadorinteligente`, (error, stdout, stderr) => {
              if(error){
                console.log('Error: ' + error)
                //erro=1;
              }
              if(stderr){
                console.log('Stderr: ' + stderr)
              }
              if(stdout){
                console.log('Stdout: ' + stdout)
              }
            })
            childRestore.on('exit', function(){
              resolve(1)
            })
          })
        }

        /*function restoreDatabaseAdmin() {
          return new Promise(resolve => {
            var childRestore = child_restoreSystem.exec(`mongorestore --username "administrador" --password "EapIk9oc" --authenticationDatabase admin --db admin --drop dboriginal/admin`, (error, stdout, stderr) => {
              if(error){
                console.log('Error: ' + error)
                //erro=1;
              }
              if(stderr){
                console.log('Stderr: ' + stderr)
              }
              if(stdout){
                console.log('Stdout: ' + stdout)
              }
            })
            childRestore.on('exit', function(){
              resolve(1)
            })
          })
        }*/

        function restoreInterface() {
          return new Promise(resolve => {
            var childRestore = child_restoreSystem.exec(`yes | cp -rf interfaceoriginal/* .`, (error, stdout, stderr) => {
              if(error){
                console.log('Error: ' + error)
                //erro=1;
              }
              if(stderr){
                console.log('Stderr: ' + stderr)
              }
              if(stdout){
                console.log('Stdout: ' + stdout)
              }
            })
            childRestore.on('exit', function(){
              resolve(1)
            })
          })
        }

        //Gera e envia relatório de checkout
        function geraRelatorioCheckout(){
          if (redeWifiOk==1){
            Evento.find({devolucao: 0}/*{$or:[{dataRetirada:data}, {dataDevolucao:data}]}*/)
            .select('referencia descricao gaveta dataRetirada horaRetirada usuario setor -_id')
            .lean().populate("eventos").sort({_id: "asc"}).then((eventos) => {
              if(eventos.length>0){
                const csv = new ObjectsToCsv(eventos);
                console.log('Gerando relatorio...')
                csv.toDisk('ferramentasAusentes.csv').then(function(){
                  console.log('Iniciando envio de e-mail...')
                  enviaEmailCheckout(1)
                })
              }
              else{
                enviaEmailCheckout(0)
                console.log('Nenhuma ferramenta ausente')
              }
            }).catch((err) => {
              console.log(err)
            })
          }
          else{
            console.log('Wifi não conectado...')
            socket.emit('resultadoRelatorioCheckout', 0)
          }
        }

        function enviaEmailCheckout(ferramentasAusentes){
          if(validarEmail(configuracaoAtual.email)!=1 && validarEmail(configuracaoAtual.email2)!=1 && validarEmail(configuracaoAtual.email3)!=1){
            console.log('Não há nenhum e-mail cadastrado')
            socket.emit('resultadoRelatorioCheckout', 2)
          }
          else{
            var email
            var usuarioatual = userLogged('showUserdata')
            var horario = getHorario();
            var data = getData();
            var textoEmail = ''
            textoEmail = 'Usuário: ' + usuarioatual.login + '\nSetor: ' + usuarioatual.setor;
            textoEmail = textoEmail + '\nHorário: ' + horario + '\nData: ' + data + '\n';
            let iterable = [1,2,3,4,5,6,7,8]
            for(const value of iterable){ //Para executar ordenadamente, o laço for deve ser assim
              if (ItensErrados[value-1]==404){ //Estado desconhecido
                textoEmail = textoEmail + '\nGaveta ' + String(value) + ': Estado das ferramentas desconhecido';
              }
              else if (ItensErrados[value-1]==0){ //Nenhuma ferramenta ausente
                textoEmail = textoEmail + '\nGaveta ' + String(value) + ': Todas as ferramentas presentes';
              }
              else{ //Exibe quantidade de ferramentas ausentes
                textoEmail = textoEmail + '\nGaveta ' + String(value) + ': ' + String(ItensErrados[value-1]) + " ferramentas ausentes";
              }
            }
            console.log(textoEmail)
            if(ferramentasAusentes==1){
              pathToAttachment = `./ferramentasAusentes.csv`;
              attachment = fs.readFileSync(pathToAttachment).toString("base64");
              email = {
                to: [configuracaoAtual.email, configuracaoAtual.email2, configuracaoAtual.email3],
                //to: emailTramontina,
                from: 'sistemainteligente.pro@tramontina.com',
                //bcc: emailTramontina,
                subject:'Relatório instantâneo SmartManager4.0',
                text: 'Relatório ' + String(configuracaoAtual.identificacao) + '\n\n' + textoEmail,
                attachments: [
                  {
                    content: attachment,
                    filename: 'Ferramentas Ausentes.csv',
                    type: 'application/csv',
                    disposition: 'attachment'
                  },
                ],
              }
            }
            else{
              email = {
                to: [configuracaoAtual.email, configuracaoAtual.email2, configuracaoAtual.email3],
                //to: emailTramontina,
                from: 'sistemainteligente.pro@tramontina.com',
                //bcc: emailTramontina,
                subject:'Relatório instantâneo SmartManager4.0',
                text: 'Relatório ' + String(configuracaoAtual.identificacao) + '\n\n' + textoEmail,
              }
            }
            sgMail
              .send(email)
              .then(() => {
                console.log('Email enviado com sucesso')
                socket.emit('resultadoRelatorioCheckout', 1)
              })
              .catch((error) => {
                console.log('Erro ao enviar email: ' + error);
                socket.emit('resultadoRelatorioCheckout', 3)
              })
          }
        }

        //End ON Events
        //send a message to ALL connected clients
        socket.emit('passLength', senha);
    });
    console.log(teste)
    return router;
};

module.exports = method
