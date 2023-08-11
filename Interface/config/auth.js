const localStrategy = require("passport-local").Strategy
const mongoose = require("mongoose")
const bcrypt = require("bcryptjs")
var FuncSenha = require("../routes/senha");
//Model de usuário
require("../models/Usuario")
const Usuario = mongoose.model("usuarios")
var senha = ""
var nome = ""
//var tentativasInvalidas = 0

const method = function(passport){
  passport.use(new localStrategy({
    usernameField: 'login',
    passwordField: 'senha',
  },
    function(login, senha, done) {
      if (senha.length<=3){
          console.log("Verificando rfid... ")
          Usuario.findOne({rfid: login}).then((usuario) => {
            if(!usuario){
              return done(null, false, {message: "Este cartão não está cadastrado"})
            }
            else{
              FuncSenha("resetTentativas")
              return done(null, usuario)
            }
          })
      }
      else{
        Usuario.findOne({ username: login }, function (err, user) {
          console.log("Verificando login e senha... ")
          Usuario.findOne({login: login}).then((usuario) => {
            if(!usuario){
              return done(null, false, {message: "Esta conta não existe"})
            }
            bcrypt.compare(senha, usuario.senha, (erro, batem) => {
              if(batem){
                //tentativasInvalidas = 0
                FuncSenha("resetTentativas")
                return done(null, usuario)
              }else{
                //tentativasInvalidas = tentativasInvalidas+1
                //if (tentativasInvalidas<3){
                  FuncSenha("incTentativas")
                  return done(null, false, {message: "Senha incorreta"})
                //}else{
                //  tentativasInvalidas=0
                //  return done(null, false, {message: "Número de tentativas de login excedido, tente novamente mais tarde"})
                //}
              }
            })
          })
        })
      }
    }
  ))

  passport.serializeUser((usuario, done) => {
    done(null, usuario.id)
  })

  passport.deserializeUser((id, done) => {
    Usuario.findById(id, (err, usuario) =>{
      done(err, usuario)
    })
  })

}
module.exports = method
