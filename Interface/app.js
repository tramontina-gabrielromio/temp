//Carregando módulos
const express = require('express')
const handlebars = require('express-handlebars')
const bodyParser = require('body-parser')
const mongoose = require('mongoose')
const {spawn} = require('child_process');
const app = express()
const router = express.Router()

const admin = require("./routes/admin") //Vincula com o arquivo admin.js

//Descomentar a linha para alimentar o banco de dados com informações referentes às ferramentas
//const database = require("./routes/database") //Vincula com o arquivo database.js

const path = require("path")
const session = require("express-session")
const flash = require("connect-flash")

/*
require("./models/Postagem")
const Postagem = mongoose.model("postagens")
require("./models/Categoria")
const Categoria = mongoose.model("categorias")
*/
require("./models/Usuario")
const Usuario = mongoose.model("usuarios")
const usuarios = require("./routes/usuario")
const passport = require("passport")
require("./config/auth")(passport)

require("./models/Configuracoes")
const Configuracao = mongoose.model("configuracoes")

//Configura rota para Socket io
app.io = require('socket.io')();
var routes = require('./routes/sockets')(app.io);
app.use(routes)
var server = require('http').Server(app);
app.io.attach(server);

const sockets = require("./routes/sockets")
app.use("/sockets", sockets)

//Configura rota para Sockets Globais io
app.global = require('socket.io')();
var routesGlobal = require('./routes/socketglobal')(app.global);
app.use(routesGlobal)
//app.global.attach(server);
const socketglobal = require("./routes/socketglobal")
app.use("/socketglobal", socketglobal)

//Configurações
  //Sessão
  app.use(session({
    secret: "organizadorinteligente",
    resave: true,
    saveUninitialized: true
  }))

  app.use(passport.initialize()) //Obs. devem ser declarados abaixo da "session"
  app.use(passport.session())

  app.use(flash())

  //Middleware

  var d = new Date();
  var h = d.getHours();
  var m = d.getMinutes();

  app.use((req, res, next) => {
    res.locals.success_msg = req.flash("success_msg") //Cria variáveis globais para armazenamento de mensagens
    res.locals.error_msg = req.flash("error_msg")
    res.locals.error = req.flash("error")
    res.locals.user = req.user || null;
    if(req.user||null){
      res.locals.admin = req.user.nivel
    }else{
      res.locals.admin = 0
    }
    next()
  })

  //Body parser
  app.use(bodyParser.urlencoded({extended: true}))
  app.use(bodyParser.json()) //Necessário para trabalhar com diretórios e manipular pastas
  //Handlebars
  app.engine('handlebars', handlebars({defaultLayout: 'main'}))
  app.set('view engine', 'handlebars')
  //Mongoose
  mongoose.Promise = global.Promise
  conectaDatabase()
  async function conectaDatabase(){
	  await mongoose.connect("mongodb://administrador:EapIk9oc@127.0.0.1/organizadorinteligente?authSource=admin", {
		useNewUrlParser: true,
		useUnifiedTopology: true
	  }).then(() => {
		console.log("Conectado ao database")
	  }).catch((err) => {
		console.log("Erro ao se conectar: " + err)
	  })
  }
  //Public
  app.use(express.static(path.join(__dirname, "public"))) //Indica que o diretório em que estão os arquivos estáticos é a pasta public

//Rotas
/*  app.get('/', (req, res) => {
      res.render("index")
  })*/

  app.get('/', (req, res) => {
    req.logout(function (err){
      if (err){return next(err);}
    });
    res.render("index", {layout:false}); //Rendeniza a tela sem o template utilizado nas demais telas
  })

  app.get('/skiploading', (req, res) => {
    res.render("skiploading"); //Rendeniza a tela sem o template utilizado nas demais telas
  })

  app.get("/restaurar",  (req, res) => {
    res.render("restaurar")
  })

  app.get('/404', (req, res) => {
    console.log("Error: " + req.query.data)
    res.render("404", {erro:req.query.data});
  })

  app.get('/usuarios', (req, res) => {
    req.logout(function (err){
      if (err){return next(err);}
    });
    Usuario.find({$or:[{nivel:0}, {nivel:1}, {nivel:2}]}).lean().populate("usuarios").sort({login: "asc"}).then((usuarios) => {
      Configuracao.findOne({_id:'000000000000000000000001'}).lean().populate("configuracoes").then((configuracoes) => {
        res.render("usuarios/usuarios", {usuarios: usuarios, configuracoes: configuracoes})
      }).catch((err) => {
        req.flash("error_msg", "Houve um erro interno")
        res.redirect("/404?data=" + err);
      })
    }).catch((err) => {
      req.flash("error_msg", "Houve um erro interno")
      res.redirect("/404?data=" + err);
    })
  })

//  app.get('/loading', (req, res) => {
//    res.render("loading", {layout:false}); //Rendeniza a tela de loading enquanto carrega as redes neurais
//  })

//  app.get("/usuarios/login/:nome", (req, res) => {
//    res.render("usuarios/login", {nome: req.params.nome})
//  })


//Configura wifi
/*const wifi = require('node-wifi')

wifi.init({
  iface: null
})

wifi.scan((error, networks) => {
  if (error){
    console.log("Erro ao procurar redes wifi: " + error);
  }else{
    console.log(networks);
  }
})

wifi.connect({
  ssid: 'Tramontina Visitantes', password: 'tramontina1911'
}, (error) => {
  if (error)
    console.log('Erro...')
  else
    console.log('Conectado...')
})*/

app.use("/admin", admin)
app.use("/usuarios", usuarios)

//start our web server and socket.io server listening
server.listen(8081, function(){
  console.log('listening on *:8081');
  ExecuteChromium(); //Descomentar para inicializar o chromium com a aplicação
});

const { exec } = require('child_process');

function ExecuteChromium(){
  exec("chromium-browser --kiosk --disable-pinch --overscroll-history-navigation=0 http://localhost:8081/ --password-store=basic %U", function(error, stdout, stderr){
    //console.log("stdout: " + stdout);
    //console.log("stderr: " + stderr);
    if (error !== null){
      console.log("exec error: " + error);
    }
  })
}