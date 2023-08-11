const express = require('express')
const router = express.Router()
const mongoose = require("mongoose")
require("../models/Usuario")
const Usuario = mongoose.model("usuarios")
require("../models/Ferramenta")
const Ferramenta = mongoose.model("ferramentas")
const bcrypt = require("bcryptjs")
const passport = require("passport")
const {admin} = require("../helpers/administrador")
const Schema = mongoose.Schema
require("../models/DadosTeste")
const DadosTeste = mongoose.model("dadosTestes")
require("../models/Configuracoes")
const Configuracao = mongoose.model("configuracoes")
const wifi = require('node-wifi')

wifi.init({
  iface: null
})

// Scan networks
/*function wifilist(){
  var aux = [];
  wifi.scan((error, networks) => {
    if (error) {
      console.log(error);
    } else {
      //console.log(networks);
      networks.forEach(function(network, indice) {
        aux[indice] = network.ssid;   
        redesWifi = network.ssid;
      });
      //console.log(aux);
      redesWifi = networks;
    }
  });
  return redesWifi;
}*/

router.get("/useroptions", admin,  (req, res) => {
  res.render("admin/useroptions")
})

/*router.get("/dateTime", admin,  (req, res) => {
  res.render("admin/dateTime")
})*/

router.get("/acessoremoto", admin,  (req, res) => {
  res.render("admin/acessoremoto")
})

router.get("/options", admin,  (req, res) => {
  Configuracao.findOne({_id:'000000000000000000000001'}).lean().populate("configuracoes").then((configuracoes) => {
    if(configuracoes){
      res.render("admin/options", {configuracoes: configuracoes})
    }
    else{
      console.log("Lista de configurações vazia...")
      const novaConfiguracao = {
        fechamento: 0,
        cartao: 0,
        desligamento: 0,
        email: '-',
        dataRelatorio: '0'}
      res.render("admin/options", {configuracoes: novaConfiguracao})
    }
    //console.log(configuracoes)
  }).catch((err) => {
    req.flash("error_msg", "Houve um erro interno")
    res.redirect("/404?data=" + err);
  })
})

router.get("/testes", admin,  (req, res) => {
  res.render("admin/testes")
})

router.get("/atualizacao", admin,  (req, res) => {
  res.render("admin/atualizacao")
})

/*wifi.scan((error, networks) => {
  if (error) {
    console.log(error);
  } else {
    console.log("Redes disponiveis: ");
    var redesWifi = new Array;
    console.log(networks);
    var cont = 0;
    networks.forEach(function(network, indice) { 
      if (network.frequency == 2462){ //Exibe apenas redes 2.4Ghz
        redesWifi[cont] = {
          ssid: network.ssid,
          security: network.security,
          signal_level: network.signal_level}
          cont = cont+1;
        }
    });
    //networks.forEach(function(network, indice) { 
    //  redesWifi = network.ssid;
    //});
    console.log(redesWifi);
  }
});*/

const { exec } = require('child_process');
var enderecoMac;
returnMAC();

/*function returnMAC() {
  const child = spawn('cat', ['/sys/class/net/wlan0/address']);
  child.stdout.on('data', (data) => {
    enderecoMac = ((String(data)).toUpperCase()).replace(/:/g, '-');
  });
}*/

function returnMAC() {
  exec('cat /sys/class/net/wlan0/address', (error, stdout, stderr)=>{
    if (stdout){
      enderecoMac = ((stdout).toUpperCase()).replace(/:/g, '-');
    }
  })
}

router.get("/wifi", admin,  (req, res) => {
  wifi.getCurrentConnections((error, network) => {
    if (network.length > 0){ //Conectado a rede, exibe opções de desconexão
      var frequency = String(network[0].frequency)
      network[0].frequency = frequency[0]+','+frequency[1]
      if (network[0].signal_level>=-60)
        network[0].signal_level = 4
      else if (network[0].signal_level>=-70)
        network[0].signal_level = 3
      else if (network[0].signal_level>=-80)
        network[0].signal_level = 2
      else
        network[0].signal_level = 1
      res.render("admin/wifi", {network: network.slice(0,1), conectado:1, mac: enderecoMac})
    }
    else{ //Desconectado a rede, exibe opções de conexão
      wifi.scan((error, networks) => {
        if (error)
          console.log(error);
        else
          res.render("admin/wifi", {network: agrupaWifi(networks), conectado:0, mac: enderecoMac})
      });
    }
  });
})

function agrupaWifi(networks){
  var redesWifi = new Array;
  var cont = 0;
  networks.forEach(function(network, indice) {
    if (network.signal_level>=-60)
      network.signal_level = 4
    else if (network.signal_level>=-70)
      network.signal_level = 3
    else if (network.signal_level>=-80)
      network.signal_level = 2
    else
      network.signal_level = 1
    var ssidExistente = 0
    redesWifi.forEach(function(redeWifi, indice2){
      if (redeWifi.ssid==network.ssid){
        ssidExistente = 1
        if(network.signal_level>redeWifi.signal_level)
          redeWifi.signal_level = network.signal_level
      }
    })
    if (ssidExistente == 0 && network.ssid != ""){ //Acrescenta rede ao novo array
      redesWifi[cont] = {
        ssid: network.ssid,
        security: network.security,
        signal_level: network.signal_level}
        cont = cont+1;
    }
  })
  console.log(redesWifi)
  return redesWifi;
}

router.get("/gavetaespecifica", admin,  (req, res) => {
  res.render("admin/gavetaespecifica")
})

router.get("/gvespecificahistorico", admin,  (req, res) => {
  DadosTeste.find({tipo:0}).lean().populate("dadosTestes").sort({_id: "desc"}).then((dadosTestes) => {
    res.render("admin/gvespecificahistorico", {eventos: dadosTestes})
  }).catch((err) => {
    req.flash("error_msg", "Houve um erro interno")
    res.redirect("/404?data=" + err);
  })
})

router.get("/todasgvhistorico", admin,  (req, res) => {
  DadosTeste.find({tipo:1}).lean().populate("dadosTestes").sort({_id: "desc"}).then((dadosTestes) => {
    res.render("admin/todasgvhistorico", {eventos: dadosTestes})
  }).catch((err) => {
    req.flash("error_msg", "Houve um erro interno")
    res.redirect("/404?data=" + err);
  })
})

router.get("/todasgavetas", admin,  (req, res) => {
  res.render("admin/todasgavetas")
})

router.get("/adduser", admin,  (req, res) => {
  res.render("admin/adduser")
})

router.get("/edituser", admin,  (req, res) => {
  Usuario.find({$or:[{nivel:0}, {nivel:1}, {nivel:2}]}).lean().populate("usuarios").sort({login: "asc"}).then((usuarios) => {
    res.render("admin/edituser", {usuarios: usuarios, busca: ''})
  }).catch((err) => {
    req.flash("error_msg", "Houve um erro interno")
    res.redirect("/404?data=" + err);
  })
})

router.post("/edituser/buscar", admin,  (req, res) => {
  Usuario.find({$and:[{login:new RegExp(req.body.userLogin, 'i')}, {$or:[{nivel:0}, {nivel:1}, {nivel:2}]}]}).lean().populate("usuarios").sort({login: "asc"}).then((usuarios) => {
    res.render("admin/edituser", {usuarios: usuarios, busca: req.body.userLogin})
  }).catch((err) => {
    req.flash("error_msg", "Houve um erro interno")
    res.redirect("/404?data=" + err);
  })
})

function isNumber(n){
  return !isNaN(parseFloat(n)) && isFinite(n);
}

router.post("/adduser", admin, (req, res) => {
  var error = []
  if(!req.body.nome || typeof req.body.nome == undefined || typeof req.body.nome == null){
    error.push({texto: "Nome inválido"})
  }
  if(!req.body.setor || typeof req.body.setor == undefined || typeof req.body.setor == null){
    error.push({texto: "Setor inválido"})
  }
  if(req.body.login.length > 15){
    error.push({texto: "O login deve conter no máximo 15 caracteres"})
  }
  else if(!req.body.login || typeof req.body.login == undefined || typeof req.body.login == null){
    error.push({texto: "Login inválido"})
  }
  if(req.body.senha.length < 4 || req.body.senha.length > 4){
    error.push({texto: "A senha deve conter 4 caracteres"})
  }
  else if(!isNumber(req.body.senha)){
    error.push({texto: "A senha deve conter apenas números"})
  }
  else if(!req.body.senha || typeof req.body.senha == undefined || typeof req.body.senha == null){
    error.push({texto: "Senha inválida"})
  }
  if(error.length > 0){
    var usuario = new Schema({nome:{type:String},setor:{type:String},login:{type:String},rfid:{type:String}
    ,gv1:{type:Boolean},gv2:{type:Boolean},gv3:{type:Boolean},gv4:{type:Boolean},gv5:{type:Boolean},gv6:{type:Boolean},gv7:{type:Boolean},gv8:{type:Boolean}})
    usuario.nome = req.body.nome
    usuario.setor = req.body.setor
    usuario.login = req.body.login
    usuario.rfid = req.body.rfid
    usuario.gv1 = req.body.gv1
    usuario.gv2 = req.body.gv2
    usuario.gv3 = req.body.gv3
    usuario.gv4 = req.body.gv4
    usuario.gv5 = req.body.gv5
    usuario.gv6 = req.body.gv6
    usuario.gv7 = req.body.gv7
    usuario.gv8 = req.body.gv8
    res.render("admin/adduser", {error: error, usuario: usuario})
  }else{
    Usuario.findOne({$or:[{login: req.body.login}, {$and:[{rfid: req.body.rfid}, {rfid:{$ne:""}}]}]}).then((usuarioExistente) => {
      if (usuarioExistente){
        if (usuarioExistente.login == req.body.login){
          //req.flash("error_msg", "Já existe uma conta registrada com esse login")
          //res.redirect("/admin/adduser")}
          error.push({texto: "Já existe uma conta registrada com esse login"})
          var usuario = new Schema({nome:{type:String},setor:{type:String},login:{type:String},rfid:{type:String}
          ,gv1:{type:Boolean},gv2:{type:Boolean},gv3:{type:Boolean},gv4:{type:Boolean},gv5:{type:Boolean},gv6:{type:Boolean},gv7:{type:Boolean},gv8:{type:Boolean}})
          usuario.nome = req.body.nome
          usuario.setor = req.body.setor
          usuario.login = ""
          usuario.rfid = req.body.rfid
          usuario.gv1 = req.body.gv1
          usuario.gv2 = req.body.gv2
          usuario.gv3 = req.body.gv3
          usuario.gv4 = req.body.gv4
          usuario.gv5 = req.body.gv5
          usuario.gv6 = req.body.gv6
          usuario.gv7 = req.body.gv7
          usuario.gv8 = req.body.gv8
          res.render("admin/adduser", {error: error, usuario: usuario})}
        else{
          console.log("adicionando 5...")
          //req.flash("error_msg", "Já existe uma conta registrada com esse cartão RFID")
          //res.redirect("/admin/adduser")}
          error.push({texto: "Já existe uma conta registrada com esse cartão RFID"})
          var usuario = new Schema({nome:{type:String},setor:{type:String},login:{type:String},rfid:{type:String}
          ,gv1:{type:Boolean},gv2:{type:Boolean},gv3:{type:Boolean},gv4:{type:Boolean},gv5:{type:Boolean},gv6:{type:Boolean},gv7:{type:Boolean},gv8:{type:Boolean}})
          usuario.nome = req.body.nome
          usuario.setor = req.body.setor
          usuario.login = req.body.login
          usuario.rfid = ""
          usuario.gv1 = req.body.gv1
          usuario.gv2 = req.body.gv2
          usuario.gv3 = req.body.gv3
          usuario.gv4 = req.body.gv4
          usuario.gv5 = req.body.gv5
          usuario.gv6 = req.body.gv6
          usuario.gv7 = req.body.gv7
          usuario.gv8 = req.body.gv8
          res.render("admin/adduser", {error: error, usuario: usuario})}
        }
      else{
        const novoUsuario = new Usuario({
          nome: req.body.nome,
          setor: req.body.setor,
          login: req.body.login,
          senha: req.body.senha,
          rfid: req.body.rfid,
          //nivel: 2,
          gv1: req.body.gv1,
          gv2: req.body.gv2,
          gv3: req.body.gv3,
          gv4: req.body.gv4,
          gv5: req.body.gv5,
          gv6: req.body.gv6,
          gv7: req.body.gv7,
          gv8: req.body.gv8
        })
        //encripta a senha
        bcrypt.genSalt(10, (erro, salt) =>{
          bcrypt.hash(novoUsuario.senha, salt, (erro, hash) => {
            if(erro){
              req.flash("error_msg", "Houve um erro durante o salvamento do usuário")
              res.redirect("/")
            }
            novoUsuario.senha = hash
            novoUsuario.save().then(() => {
              req.flash("success_msg", "Usuario criado com sucesso!")
              res.redirect("/admin/useroptions")
            }).catch((err) => {
              req.flash("error_msg", "Houve um erro ao criar o usuário, tente novamente")
              res.redirect("/admin/adduser")
            })
          })
        })
      }
    }).catch((err) => {
      req.flash("error_msg", "Houve um erro interno")
      res.redirect("/")
    })
  }
})

router.post("/edituser/editar", admin, (req, res) => {
  if(!req.body.log){
    req.flash("error_msg", "Nenhum usuário selecionado")
    res.redirect("/admin/edituser")
  }
  else{
    Usuario.findOne({login:req.body.log}).lean().then((usuario) => {
      if(!usuario){
        res.render("admin/edituser/", {error: error})
      }
      else{
        res.render("admin/editar", {usuario: usuario})
      }
    }).catch((err) => {
      req.flash("error_msg", "Este usuario não existe")
      res.redirect("/admin/edituser")
    })
  }
})

router.post("/edituser/deletar", admin, (req, res) => {
  if(!req.body.log){
    req.flash("error_msg", "Nenhum usuário selecionado")
    res.redirect("/admin/edituser")
  }
  else{
    if(req.body.nivel!='2'){
      console.log('nivel: ' + req.body.nivel)
      Usuario.remove({login:req.body.log}).then(() => { //body.id porque esse parâmetro virá do formulário oculto com o id criado no categorias.handlebars
        req.flash("success_msg", "Usuario deletado com sucesso!")
        res.redirect("/admin/edituser")
      }).catch((err) => {
        req.flash("error_msg", "Houve um erro ao deletar o usuário")
        res.redirect("/admin/edituser")
      })
    }
    else{
      req.flash("error_msg", "O usuário selecionado não pode ser excluido")
      res.redirect("/admin/edituser")
    }
  }
})

router.post("/edituser/editar/salvar", admin, (req, res) => {
  var error = []
  var auxManterSenha = 0
  if(!req.body.nome || typeof req.body.nome == undefined || typeof req.body.nome == null){
    error.push({texto: "Nome inválido"})
  }
  if(!req.body.setor || typeof req.body.setor == undefined || typeof req.body.setor == null){
    error.push({texto: "Setor inválido"})
  }
  if(req.body.login.length > 15){
    error.push({texto: "O login deve conter no máximo 15 caracteres"})
  }
  else if(!req.body.login || typeof req.body.login == undefined || typeof req.body.login == null){
    error.push({texto: "Login inválido"})
  }
  if (req.body.senha.length == 0){
    auxManterSenha = 1;
  }
  else if(req.body.senha.length < 4 || req.body.senha.length > 4){
    error.push({texto: "A senha deve conter 4 caracteres"})
  }
  else if(!isNumber(req.body.senha)){
    error.push({texto: "A senha deve conter apenas números"})
  }
  else if(!req.body.senha || typeof req.body.senha == undefined || typeof req.body.senha == null){
    error.push({texto: "Senha inválida"})
  }
  if(error.length > 0){
    var usuario = new Schema({_id:{type:String},nome:{type:String},setor:{type:String},login:{type:String},rfid:{type:String}})
    usuario._id = req.body.id
    usuario.nome = req.body.nome
    usuario.setor = req.body.setor
    usuario.login = req.body.login
    usuario.rfid = req.body.rfid
    usuario.gv1 = req.body.gv1
    usuario.gv2 = req.body.gv2
    usuario.gv3 = req.body.gv3
    usuario.gv4 = req.body.gv4
    usuario.gv5 = req.body.gv5
    usuario.gv6 = req.body.gv6
    usuario.gv7 = req.body.gv7
    usuario.gv8 = req.body.gv8
    res.render("admin/editar", {error: error, usuario: usuario})
  }else{
    //({$or:[{login: req.body.login}, {$and:[{rfid: req.body.rfid}, {rfid:{$ne:""}}]}]})
    //Usuario.findOne({$or:[{login: req.body.login, _id:{$ne:req.body.id}}, {rfid: req.body.rfid, _id:{$ne:req.body.id}, rfid:{$ne:""}}]}).then((usuarioExistente) => {
    Usuario.findOne({$or:[{$and:[{_id:{$ne:req.body.id}}, {login: req.body.login}]}, {$and:[{_id:{$ne:req.body.id}}, {rfid: req.body.rfid}, {rfid:{$ne:""}}]}]}).then((usuarioExistente) => {  
      if (usuarioExistente){
        if (usuarioExistente.login == req.body.login){
          error.push({texto: "Já existe uma conta registrada com esse login"})
          var usuario = new Schema({_id:{type:String},nome:{type:String},setor:{type:String},login:{type:String},rfid:{type:String}})
          usuario._id = req.body.id
          usuario.nome = req.body.nome
          usuario.setor = req.body.setor
          usuario.login = ""
          usuario.rfid = req.body.rfid
          usuario.gv1 = req.body.gv1
          usuario.gv2 = req.body.gv2
          usuario.gv3 = req.body.gv3
          usuario.gv4 = req.body.gv4
          usuario.gv5 = req.body.gv5
          usuario.gv6 = req.body.gv6
          usuario.gv7 = req.body.gv7
          usuario.gv8 = req.body.gv8
          res.render("admin/editar", {error: error, usuario: usuario})}
        else if (usuarioExistente.rfid == req.body.rfid){
          error.push({texto: "Já existe uma conta registrada com esse cartão RFID"})
          var usuario = new Schema({_id:{type:String},nome:{type:String},setor:{type:String},login:{type:String},rfid:{type:String}})
          usuario._id = req.body.id
          usuario.nome = req.body.nome
          usuario.setor = req.body.setor
          usuario.login = req.body.login
          usuario.rfid = ""
          usuario.gv1 = req.body.gv1
          usuario.gv2 = req.body.gv2
          usuario.gv3 = req.body.gv3
          usuario.gv4 = req.body.gv4
          usuario.gv5 = req.body.gv5
          usuario.gv6 = req.body.gv6
          usuario.gv7 = req.body.gv7
          usuario.gv8 = req.body.gv8
          res.render("admin/editar", {error: error, usuario: usuario})}
      }
      else {
        Usuario.findOne({_id: req.body.id}).then((usuario) => {
          usuario.nome = req.body.nome
          usuario.setor = req.body.setor
          usuario.login = req.body.login
          usuario.rfid = req.body.rfid
          usuario.gv1 = req.body.gv1
          usuario.gv2 = req.body.gv2
          usuario.gv3 = req.body.gv3
          usuario.gv4 = req.body.gv4
          usuario.gv5 = req.body.gv5
          usuario.gv6 = req.body.gv6
          usuario.gv7 = req.body.gv7
          usuario.gv8 = req.body.gv8
          if (auxManterSenha==1){
            usuario.senha = usuario.senha
            usuario.save().then(() => {
              req.flash("success_msg", "Usuario editado com sucesso!")
              res.redirect("/admin/edituser")
            }).catch((err) => {
              req.flash("error_msg", "Houve um erro ao salvar o usuário, tente novamente")
              res.redirect("/admin/edituser")
            })
          }
          else{
            usuario.senha = req.body.senha
            //encripta a senha
            bcrypt.genSalt(10, (erro, salt) =>{
              bcrypt.hash(usuario.senha, salt, (erro, hash) => {
                if(erro){
                  req.flash("error_msg", "Houve um erro ao salvar o usuário, tente novamente")
                  res.redirect("/")
                }
                usuario.senha = hash
                usuario.save().then(() => {
                  req.flash("success_msg", "Usuario editado com sucesso!")
                  res.redirect("/admin/edituser")
                }).catch((err) => {
                  req.flash("error_msg", "Houve um erro ao salvar o usuário, tente novamente")
                  res.redirect("/admin/edituser")
                })
              })
            })
          }
        }).catch((err) => {
          req.flash("error_msg", "Houve um erro ao editar o usuario")
          console.log("Erro ao editar o usuario: " + err)
          res.redirect("/admin/edituser")
        })
      }
    })
  }
})

module.exports = router
