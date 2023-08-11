const express = require('express')
const router = express.Router()
const mongoose = require('mongoose')
require("../models/Evento")
const Evento = mongoose.model("eventos")
require("../models/Ferramenta")
const Ferramenta = mongoose.model("ferramentas")
var userLogged = require("../routes/userLogged");
var FuncSenha = require("../routes/senha");
require("../models/Configuracoes")
const Configuracao = mongoose.model("configuracoes")
const passport = require("passport")
const {logged} = require("../helpers/logado")
//app.use(express.static('views/admin'));

var xyGav = []

try{ //Carrega coordenadas das ferramentas
  xyGav[1] = require("./xyGav1");
  xyGav[2] = require("./xyGav2");
  xyGav[3] = require("./xyGav3");
  xyGav[4] = require("./xyGav4");
  xyGav[5] = require("./xyGav5");
  xyGav[6] = require("./xyGav6");
  xyGav[7] = require("./xyGav7");
  xyGav[8] = require("./xyGav8");
}catch (error){
  console.log('Error: ' + error)
  xyGav[1] = [];
  xyGav[2] = [];
  xyGav[3] = [];
  xyGav[4] = [];
  xyGav[5] = [];
  xyGav[6] = [];
  xyGav[7] = [];
  xyGav[8] = [];
}

var reordenarUltimo = 0

router.get("/login/:login", (req, res, next) => {
      res.render("usuarios/login", {login: req.params.login})
})

router.get("/login", (req, res, next) => {
      res.render("usuarios/login")
})

router.get("/loginblocked", (req, res, next) => {
  Configuracao.findOne({_id:'000000000000000000000001'}).lean().populate("configuracoes").then((configuracoes) => {
    res.render("usuarios/loginblocked", {configuracoes: configuracoes})
  }).catch((err) => {
    req.flash("error_msg", "Houve um erro interno")
    res.redirect("/404?data=" + err);
  })
})

//router.get("/telalogout", (req, res, next) => {
//  res.render("usuarios/telalogout")
//})

router.get("/teladesligar", (req, res, next) => {
  res.render("usuarios/teladesligar")
})

var qtdRegistros = 100 //Quantidade de registros para exibir no historico completo

router.get("/historico", (req, res) => {
  var usuarioatual = userLogged('showUserdata')
  var buscaAcessiveis = []
  reordenarUltimo = 0
  if (usuarioatual.gv1)
    buscaAcessiveis.push({gaveta:1})
  if (usuarioatual.gv2)
    buscaAcessiveis.push({gaveta:2})
  if (usuarioatual.gv3)
    buscaAcessiveis.push({gaveta:3})
  if (usuarioatual.gv4)
    buscaAcessiveis.push({gaveta:4})
  if (usuarioatual.gv5)
    buscaAcessiveis.push({gaveta:5})
  if (usuarioatual.gv6)
    buscaAcessiveis.push({gaveta:6})
  if (usuarioatual.gv7)
    buscaAcessiveis.push({gaveta:7})
  if (usuarioatual.gv8)
    buscaAcessiveis.push({gaveta:8})

  if (buscaAcessiveis.length>0){
    Evento.countDocuments({$or:buscaAcessiveis}, function(err,count){
      Evento.find({$or:buscaAcessiveis}).lean().populate("eventos").sort({_id: "desc"}).limit(qtdRegistros).skip(0).then((eventos) => {
        res.render("usuarios/historico", {eventos: eventos, busca: '', skip: 0, pgi:1, pgf:parseInt(count/qtdRegistros)+1})
      }).catch((err) => {
        req.flash("error_msg", "Houve um erro interno")
        res.redirect("/404?data=" + err);
      })
    })
  }
  else
    res.render("usuarios/historico", {eventos: null, busca: '', skip: 0, pgi:1, pgf:1})
})

//Evento.countDocuments({}, function(err,count){
//  console.log("Total de eventos: " + count)
//})

router.post("/historico/skipm", (req, res) => {
  //console.log('Busca: ' + req.body.dataSearch)
  var usuarioatual = userLogged('showUserdata')
  var buscaAcessiveis = []
  if (usuarioatual.gv1)
    buscaAcessiveis.push({gaveta:1})
  if (usuarioatual.gv2)
    buscaAcessiveis.push({gaveta:2})
  if (usuarioatual.gv3)
    buscaAcessiveis.push({gaveta:3})
  if (usuarioatual.gv4)
    buscaAcessiveis.push({gaveta:4})
  if (usuarioatual.gv5)
    buscaAcessiveis.push({gaveta:5})
  if (usuarioatual.gv6)
    buscaAcessiveis.push({gaveta:6})
  if (usuarioatual.gv7)
    buscaAcessiveis.push({gaveta:7})
  if (usuarioatual.gv8)
    buscaAcessiveis.push({gaveta:8})

  if (buscaAcessiveis.length>0){
    Evento.find({$and:[ {$or:buscaAcessiveis}, {$or:[{referencia:new RegExp(req.body.dataSearch, 'i')}, {descricao:new RegExp(req.body.dataSearch, 'i')}]} ]}).countDocuments({}, function(err,count){
      //console.log("Total de eventos: " + count)
      var skip = Number(req.body.skip)-qtdRegistros
      if (skip<0)
        skip=0;
      Evento.find({$and:[ {$or:buscaAcessiveis}, {$or:[{referencia:new RegExp(req.body.dataSearch, 'i')}, {descricao:new RegExp(req.body.dataSearch, 'i')}]} ]}).lean().populate("eventos").sort({_id: "desc"}).limit(qtdRegistros).skip(skip).then((eventos) => {
        res.render("usuarios/historico", {eventos: eventos, busca: req.body.dataSearch, skip:skip, pgi:parseInt(skip/qtdRegistros)+1, pgf:parseInt(count/qtdRegistros)+1})
      }).catch((err) => {
        req.flash("error_msg", "Houve um erro interno")
        res.redirect("/404?data=" + err);
      })
    })
  }
  else
    res.render("usuarios/historico", {eventos: null, busca: req.body.dataSearch, skip: 0, pgi:1, pgf:1})
})

router.post("/historico/skipp", (req, res) => {
  //console.log('Busca: ' + req.body.dataSearch)
  var usuarioatual = userLogged('showUserdata')
  var buscaAcessiveis = []
  if (usuarioatual.gv1)
    buscaAcessiveis.push({gaveta:1})
  if (usuarioatual.gv2)
    buscaAcessiveis.push({gaveta:2})
  if (usuarioatual.gv3)
    buscaAcessiveis.push({gaveta:3})
  if (usuarioatual.gv4)
    buscaAcessiveis.push({gaveta:4})
  if (usuarioatual.gv5)
    buscaAcessiveis.push({gaveta:5})
  if (usuarioatual.gv6)
    buscaAcessiveis.push({gaveta:6})
  if (usuarioatual.gv7)
    buscaAcessiveis.push({gaveta:7})
  if (usuarioatual.gv8)
    buscaAcessiveis.push({gaveta:8})

  if (buscaAcessiveis.length>0){
    Evento.find({$or:buscaAcessiveis, descricao:new RegExp(req.body.dataSearch, 'i')}).countDocuments({}, function(err,count){
      //console.log("Total de eventos: " + count)
      var skip = Number(req.body.skip)+qtdRegistros
      if (skip>count)
        skip=skip-qtdRegistros;
      Evento.find({$or:buscaAcessiveis, descricao:new RegExp(req.body.dataSearch, 'i')}).lean().populate("eventos").sort({_id: "desc"}).limit(qtdRegistros).skip(skip).then((eventos) => {
        res.render("usuarios/historico", {eventos: eventos, busca: req.body.dataSearch, skip:skip, pgi:parseInt(skip/qtdRegistros)+1, pgf:parseInt(count/qtdRegistros)+1})
      }).catch((err) => {
        req.flash("error_msg", "Houve um erro interno")
        res.redirect("/404?data=" + err);
      })
    })
  }
  else
    res.render("usuarios/historico", {eventos: null, busca: req.body.dataSearch, skip: 0, pgi:1, pgf:1})
})

router.post("/historico/buscar", (req, res) => {
  //console.log('Busca: ' + req.body.dataSearch)
  var usuarioatual = userLogged('showUserdata')
  var buscaAcessiveis = []
  reordenarUltimo = 0
  if (usuarioatual.gv1)
    buscaAcessiveis.push({gaveta:1})
  if (usuarioatual.gv2)
    buscaAcessiveis.push({gaveta:2})
  if (usuarioatual.gv3)
    buscaAcessiveis.push({gaveta:3})
  if (usuarioatual.gv4)
    buscaAcessiveis.push({gaveta:4})
  if (usuarioatual.gv5)
    buscaAcessiveis.push({gaveta:5})
  if (usuarioatual.gv6)
    buscaAcessiveis.push({gaveta:6})
  if (usuarioatual.gv7)
    buscaAcessiveis.push({gaveta:7})
  if (usuarioatual.gv8)
    buscaAcessiveis.push({gaveta:8})

  if (buscaAcessiveis.length>0){
    //({$and:[ {$or:buscaAcessiveis}, {$or:[{referencia:new RegExp(req.body.dataSearch, 'i')}, {descricao:new RegExp(req.body.dataSearch, 'i')}]} ]})
    Evento.find({$and:[ {$or:buscaAcessiveis}, {$or:[{referencia:new RegExp(req.body.dataSearch, 'i')}, {descricao:new RegExp(req.body.dataSearch, 'i')}]} ]}).countDocuments({}, function(err,count){
      Evento.find({$and:[ {$or:buscaAcessiveis}, {$or:[{referencia:new RegExp(req.body.dataSearch, 'i')}, {descricao:new RegExp(req.body.dataSearch, 'i')}]} ]}).lean().populate("eventos").sort({_id: "desc"}).limit(qtdRegistros).skip(0).then((eventos) => {
        res.render("usuarios/historico", {eventos: eventos, busca: req.body.dataSearch, skip: 0, pgi:1, pgf:parseInt(count/qtdRegistros)+1})
      }).catch((err) => {
        req.flash("error_msg", "Houve um erro interno")
        res.redirect("/404?data=" + err);
      })
    })
  }
  else
    res.render("usuarios/historico", {eventos: null, busca: req.body.dataSearch, skip: 0, pgi:1, pgf:1})
})

router.post("/historico/reordenar", (req, res) => {
  //console.log('Busca: ' + req.body.dataSearch)
  var usuarioatual = userLogged('showUserdata')
  var buscaAcessiveis = []
  if (usuarioatual.gv1)
    buscaAcessiveis.push({gaveta:1})
  if (usuarioatual.gv2)
    buscaAcessiveis.push({gaveta:2})
  if (usuarioatual.gv3)
    buscaAcessiveis.push({gaveta:3})
  if (usuarioatual.gv4)
    buscaAcessiveis.push({gaveta:4})
  if (usuarioatual.gv5)
    buscaAcessiveis.push({gaveta:5})
  if (usuarioatual.gv6)
    buscaAcessiveis.push({gaveta:6})
  if (usuarioatual.gv7)
    buscaAcessiveis.push({gaveta:7})
  if (usuarioatual.gv8)
    buscaAcessiveis.push({gaveta:8})

  var reordenar = {}
  if (req.body.reordenar=="referencia"){
    if (reordenarUltimo==1){
      reordenarUltimo = 0
      reordenar={referencia: "desc"}}
    else {
      reordenarUltimo = 1
      reordenar={referencia: "asc"}}
  }
  else if (req.body.reordenar=="descricao"){
    if (reordenarUltimo==2){
      reordenarUltimo = 0
      reordenar={descricao: "desc"}}
    else {
      reordenarUltimo = 2
      reordenar={descricao: "asc"}}
  }
  else if (req.body.reordenar=="gaveta"){
    if (reordenarUltimo==3){
      reordenarUltimo = 0
      reordenar={gaveta: "desc"}}
    else {
      reordenarUltimo = 3
      reordenar={gaveta: "asc"}}
  }
  else if (req.body.reordenar=="dataRetirada"){
    if (reordenarUltimo==4){
      reordenarUltimo = 0
      reordenar={dataRetirada: "desc"}}
    else {
      reordenarUltimo = 4
      reordenar={dataRetirada: "asc"}}
  }
  else if (req.body.reordenar=="horaRetirada"){
    if (reordenarUltimo==5){
      reordenarUltimo = 0
      reordenar={horaRetirada: "desc"}}
    else {
      reordenarUltimo = 5
      reordenar={horaRetirada: "asc"}}
  }
  else if (req.body.reordenar=="dataDevolucao"){
    if (reordenarUltimo==6){
      reordenarUltimo = 0
      reordenar={dataDevolucao: "desc"}}
    else {
      reordenarUltimo = 6
      reordenar={dataDevolucao: "asc"}}
  }
  else if (req.body.reordenar=="horaDevolucao"){
    if (reordenarUltimo==7){
      reordenarUltimo = 0
      reordenar={horaDevolucao: "desc"}}
    else {
      reordenarUltimo = 7
      reordenar={horaDevolucao: "asc"}}
  }
  else if (req.body.reordenar=="usuario"){
    if (reordenarUltimo==8){
      reordenarUltimo = 0
      reordenar={usuario: "desc"}}
    else {
      reordenarUltimo = 8
      reordenar={usuario: "asc"}}
  }
  else if (req.body.reordenar=="setor"){
    if (reordenarUltimo==9){
      reordenarUltimo = 0
      reordenar={setor: "desc"}}
    else {
      reordenarUltimo = 9
      reordenar={setor: "asc"}}
  }

  if (buscaAcessiveis.length>0){
    Evento.find({$and:[ {$or:buscaAcessiveis}, {$or:[{referencia:new RegExp(req.body.dataSearch, 'i')}, {descricao:new RegExp(req.body.dataSearch, 'i')}]} ]}).countDocuments({}, function(err,count){
      Evento.find({$and:[ {$or:buscaAcessiveis}, {$or:[{referencia:new RegExp(req.body.dataSearch, 'i')}, {descricao:new RegExp(req.body.dataSearch, 'i')}]} ]}).lean().populate("eventos").sort(reordenar).limit(qtdRegistros).skip(0).then((eventos) => {
        res.render("usuarios/historico", {eventos: eventos, /*reordenar: reordenar,*/ busca: req.body.dataSearch, skip: 0, pgi:1, pgf:parseInt(count/qtdRegistros)+1})
      }).catch((err) => {
        req.flash("error_msg", "Houve um erro interno")
        res.redirect("/404?data=" + err);
      })
    })
  }
  else
    res.render("usuarios/historico", {eventos: null, busca: req.body.dataSearch, skip: 0, pgi:1, pgf:1})
})

router.get("/tools",  (req, res) => {
  var usuarioatual = userLogged('showUserdata')
  var buscaAcessiveis = []
  if (usuarioatual.gv1)
    buscaAcessiveis.push({gaveta:1})
  if (usuarioatual.gv2)
    buscaAcessiveis.push({gaveta:2})
  if (usuarioatual.gv3)
    buscaAcessiveis.push({gaveta:3})
  if (usuarioatual.gv4)
    buscaAcessiveis.push({gaveta:4})
  if (usuarioatual.gv5)
    buscaAcessiveis.push({gaveta:5})
  if (usuarioatual.gv6)
    buscaAcessiveis.push({gaveta:6})
  if (usuarioatual.gv7)
    buscaAcessiveis.push({gaveta:7})
  if (usuarioatual.gv8)
    buscaAcessiveis.push({gaveta:8})

  if (buscaAcessiveis.length>0){
    Ferramenta.find({$or:buscaAcessiveis}).lean().populate("ferramentas").sort({_id: "asc"}).then((ferramentas) => {
      res.render("usuarios/tools", {ferramentas: ferramentas, busca: '', usuario: usuarioatual})
    }).catch((err) => {
      req.flash("error_msg", "Houve um erro interno")
      res.redirect("/404?data=" + err);
    })
  }
  else
    res.render("usuarios/tools", {ferramentas: null, busca: ''})
})

router.post("/tools/buscar",  (req, res) => {
  var usuarioatual = userLogged('showUserdata')
  var buscaAcessiveis = []
  if (usuarioatual.gv1)
    buscaAcessiveis.push({gaveta:1})
  if (usuarioatual.gv2)
    buscaAcessiveis.push({gaveta:2})
  if (usuarioatual.gv3)
    buscaAcessiveis.push({gaveta:3})
  if (usuarioatual.gv4)
    buscaAcessiveis.push({gaveta:4})
  if (usuarioatual.gv5)
    buscaAcessiveis.push({gaveta:5})
  if (usuarioatual.gv6)
    buscaAcessiveis.push({gaveta:6})
  if (usuarioatual.gv7)
    buscaAcessiveis.push({gaveta:7})
  if (usuarioatual.gv8)
    buscaAcessiveis.push({gaveta:8})

  if (buscaAcessiveis.length>0){
    //({$or:buscaAcessiveis, descricao:new RegExp(req.body.toolSearch, 'i')})
    //({$or:buscaAcessiveis, $or:[{descricao:new RegExp(req.body.toolSearch, 'i')}, {referencia:new RegExp(req.body.toolSearch, 'i')}]})
    //({$and:[{login:new RegExp(req.body.userLogin, 'i')}, {$or:[{nivel:0}, {nivel:1}, {nivel:2}]} ]})
    //Obs. Estrutura: ({$or:[{gaveta:1},{gaveta:2},{gaveta:3}...], descricao...})
    Ferramenta.find({$and:[ {$or:buscaAcessiveis}, {$or:[{referencia:new RegExp(req.body.toolSearch, 'i')}, {descricao:new RegExp(req.body.toolSearch, 'i')}]} ]}).lean().populate("ferramentas").sort({_id: "asc"}).then((ferramentas) => {
      res.render("usuarios/tools", {ferramentas: ferramentas, busca: req.body.toolSearch, usuario: usuarioatual})
    }).catch((err) => {
      req.flash("error_msg", "Houve um erro interno")
      res.redirect("/404?data=" + err);
    })
  }
  else
    res.render("usuarios/tools", {ferramentas: null, busca: req.body.toolSearch})
})

router.get("/retiradas:id",  (req, res) => {
  Ferramenta.find({gaveta:req.params.id, desconhecido:1}).lean().populate("ferramentas").sort({_id: "asc"}).then((ferramentas) => {
    if (ferramentas.length > 0)
      res.render("usuarios/retiradas", {ferramentas: null, retiradas:'?'})
    else{
      Ferramenta.find({gaveta:req.params.id, estado:0, desconhecido:0}).lean().populate("ferramentas").sort({_id: "asc"}).then((ferramentas) => {
        res.render("usuarios/retiradas", {ferramentas: ferramentas, retiradas:-ferramentas.length})
      }).catch((err) => {
        req.flash("error_msg", "Houve um erro interno")
        res.redirect("/404?data=" + err);
      })
    }
  }).catch((err) => {
    req.flash("error_msg", "Houve um erro interno")
    res.redirect("/404?data=" + err);
  })
})

const fs = require("fs")

router.get("/usermanual", (req, res, next) => {
  //var data = fs.readFileSync('/home/tramontina/Downloads/Interface/views/usuarios/manual.pdf')
  //res.contentType("application/pdf")
  //res.send(data)
  res.render("usuarios/usermanual")
})

router.post("/documentacao", (req, res, next) => {
  res.render("usuarios/documentacao", {documentacao: req.body.documentacao})
})

router.get("/composicao", logged, (req, res) => {
  //userLogged('insertUserdata', req.user) //Após o login, armazena informações do usuário logado
  var usuarioatual = userLogged('showUserdata')
  res.render("usuarios/composicao", {/*usuario: req.user.nome, eventos: null, */usuario: usuarioatual})
})

router.get("/main", logged, (req, res) => {
  userLogged('insertUserdata', req.user) //Após o login, armazena informações do usuário logado
  var usuarioatual = userLogged('showUserdata')
  var buscaAcessiveis = []
  if (usuarioatual.gv1)
    buscaAcessiveis.push({gaveta:1})
  if (usuarioatual.gv2)
    buscaAcessiveis.push({gaveta:2})
  if (usuarioatual.gv3)
    buscaAcessiveis.push({gaveta:3})
  if (usuarioatual.gv4)
    buscaAcessiveis.push({gaveta:4})
  if (usuarioatual.gv5)
    buscaAcessiveis.push({gaveta:5})
  if (usuarioatual.gv6)
    buscaAcessiveis.push({gaveta:6})
  if (usuarioatual.gv7)
    buscaAcessiveis.push({gaveta:7})
  if (usuarioatual.gv8)
    buscaAcessiveis.push({gaveta:8})

  if (buscaAcessiveis.length>0){
    Evento.find({$or:buscaAcessiveis}).lean().populate("eventos").sort({/*devolucao: "asc", */_id: "desc"}).limit(15).then((eventos) => {
      res.render("usuarios/main", {/*usuario: req.user.nome, */eventos: eventos, usuario: usuarioatual})
    }).catch((err) => {
      req.flash("error_msg", "Houve um erro interno")
      res.redirect("/404?data=" + err);
    })
  }
  else
    res.render("usuarios/main", {/*usuario: req.user.nome, */eventos: null, usuario: usuarioatual})
})

router.get("/compGaveta:gaveta", logged, (req, res) => {
  var usuarioatual = userLogged('showUserdata')
  var buscaAcessiveis = []
  var arrayPosRemovidas = []
  if (usuarioatual.gv1) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')
  if (usuarioatual.gv2) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')
  if (usuarioatual.gv3) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')
  if (usuarioatual.gv4) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')
  if (usuarioatual.gv5) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')
  if (usuarioatual.gv6) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')
  if (usuarioatual.gv7) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')
  if (usuarioatual.gv8) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')

  if (buscaAcessiveis[(req.params.gaveta)-1]==1){
    Ferramenta.find({gaveta:req.params.gaveta}).lean().populate("ferramentas").sort({_id: "asc"}).then((ferramentas) => {
      ferramentas.forEach(function(ferramenta, indice){
        if (ferramenta.estado==0){
          try{
            arrayPosRemovidas.push({
              left: xyGav[req.params.gaveta][indice].left/1.734,
              top: xyGav[req.params.gaveta][indice].top/1.734,
              width: (xyGav[req.params.gaveta][indice].right-xyGav[req.params.gaveta][indice].left)/1.734,
              height: (xyGav[req.params.gaveta][indice].bottom-xyGav[req.params.gaveta][indice].top)/1.734})
            }
          catch(error){
            console.log(error)
          }
        }
      })
      res.render("usuarios/gaveta", {gaveta: req.params.gaveta, usuario: usuarioatual, ferramentas: ferramentas, pgBack: 2, arrayPosRemovidas: arrayPosRemovidas})
    }).catch((err) => {
      req.flash("error_msg", "Houve um erro interno")
      res.redirect("/404?data=" + err);
    })
  }
  else{
    req.flash("error_msg", "A gaveta não pode ser acessada pelo usuário")
    res.redirect("/404?data=" + err);
  }
})

router.get("/gaveta:gaveta", logged, (req, res) => {
  var usuarioatual = userLogged('showUserdata')
  var buscaAcessiveis = []
  var arrayPosRemovidas = []
  if (usuarioatual.gv1) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')
  if (usuarioatual.gv2) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')
  if (usuarioatual.gv3) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')
  if (usuarioatual.gv4) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')
  if (usuarioatual.gv5) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')
  if (usuarioatual.gv6) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')
  if (usuarioatual.gv7) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')
  if (usuarioatual.gv8) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')

  if (buscaAcessiveis[(req.params.gaveta)-1]==1){
    Ferramenta.find({gaveta:req.params.gaveta}).lean().populate("ferramentas").sort({_id: "asc"}).then((ferramentas) => {
      ferramentas.forEach(function(ferramenta, indice){
        if (ferramenta.estado==0){
          try{
            arrayPosRemovidas.push({
              left: xyGav[req.params.gaveta][indice].left/1.734,
              top: xyGav[req.params.gaveta][indice].top/1.734,
              width: (xyGav[req.params.gaveta][indice].right-xyGav[req.params.gaveta][indice].left)/1.734,
              height: (xyGav[req.params.gaveta][indice].bottom-xyGav[req.params.gaveta][indice].top)/1.734})
            }
          catch(error){
            console.log(error)
          }
        }
      })
      res.render("usuarios/gaveta", {gaveta: req.params.gaveta, usuario: usuarioatual, ferramentas: ferramentas, pgBack: 0, arrayPosRemovidas: arrayPosRemovidas})
    }).catch((err) => {
      req.flash("error_msg", "Houve um erro interno")
      res.redirect("/404?data=" + err);
    })
  }
  else{
    req.flash("error_msg", "A gaveta não pode ser acessada pelo usuário")
    res.redirect("/404?data=" + err);
  }
})

router.post("/gaveta", logged, (req, res) => {
  var usuarioatual = userLogged('showUserdata')
  var buscaAcessiveis = []
  var arrayPosRemovidas = []
  if (usuarioatual.gv1) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')
  if (usuarioatual.gv2) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')
  if (usuarioatual.gv3) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')
  if (usuarioatual.gv4) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')
  if (usuarioatual.gv5) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')
  if (usuarioatual.gv6) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')
  if (usuarioatual.gv7) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')
  if (usuarioatual.gv8) buscaAcessiveis.push('1')
  else buscaAcessiveis.push('0')

  if(!req.body.gav){
    req.flash("error_msg", "Nenhuma ferramenta selecionada")
    res.redirect("/usuarios/tools")
  }
  else{
    if (buscaAcessiveis[(req.body.gav)-1]==1){
      Ferramenta.find({gaveta:req.body.gav}).lean().populate("ferramentas").sort({_id: "asc"}).then((ferramentas) => {
        ferramentas.forEach(function(ferramenta, indice){
          if (ferramenta.estado==0){
            try{
              arrayPosRemovidas.push({
                left: xyGav[req.body.gav][indice].left/1.734,
                top: xyGav[req.body.gav][indice].top/1.734,
                width: (xyGav[req.body.gav][indice].right-xyGav[req.body.gav][indice].left)/1.734,
                height: (xyGav[req.body.gav][indice].bottom-xyGav[req.body.gav][indice].top)/1.734})
              }
            catch(error){
              console.log(error)
            }
          }
        })
        res.render("usuarios/gaveta", {gaveta: req.body.gav, usuario: usuarioatual, ferramentas: ferramentas, pgBack: 1, arrayPosRemovidas: arrayPosRemovidas})
      }).catch((err) => {
        req.flash("error_msg", "Houve um erro interno")
        res.redirect("/404?data=" + err);
      })
    }
    else{
      req.flash("error_msg", "A gaveta não pode ser acessada pelo usuário")
      res.redirect("/404?data=" + err);
    }
  }
})

router.get("/logout", (req, res) => {
  req.logout(function (err){
    if (err){return next(err);}
  });
  userLogged('resetUserdata')
  req.flash('success_msg', "Deslogado com sucesso!")
  res.redirect("/")
})
/*
router.post("/login",
  passport.authenticate("local", {
    //successRedirect: "/usuarios/main",
    failureRedirect: "/usuarios/login/",
    failureFlash: true
  }), function(req, res, next){
    res.redirect("/usuarios/main");
})*/

router.post("/login", (req, res, next) => {
  if (FuncSenha('showTentativas')<4){
    passport.authenticate("local", {
      successRedirect: "/usuarios/main",
      failureRedirect: "/usuarios/login/" + req.body.login,
      failureFlash: true
    })(req, res, next)
  }
  else{
    setTimeout(() => {FuncSenha("resetTentativas"); }, 500)
    passport.authenticate("local", {
      successRedirect: "/usuarios/main",
      failureRedirect: "/usuarios/loginblocked",
      failureFlash: true
    })(req, res, next)
  }
})

router.post("/loginrfid", (req, res, next) => {
  FuncSenha("resetTentativas")
  passport.authenticate("local", {
    successRedirect: "/usuarios/main",
    failureRedirect: "/usuarios",
    failureFlash: true
  })(req, res, next)
})

/*
login = () => {
  loginSubscription = this.authService.login(this.username, this.password).subscribe((res) => {
    router.navigate([this.redirect]);
    localStorage.setItem('currentUser', JSON.stringify(res.data));
    localStorage.removeItem('userSelected');
  }, () => {
    this.modal.show();
  });
}*/

module.exports = router
