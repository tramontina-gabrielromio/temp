const mongoose = require('mongoose')
const Schema = mongoose.Schema

const Configuracoes = new Schema({
  fechamento:{
    type: Number,
    required: false //0->5min, 1->10min, 2->20min, 3->30min, 4->nunca
  },
  cartao:{
    type: Number,
    required: true //0->desabilitado, 1->habilitado
  },
  desligamento:{
    type: Number,
    required: false //0->15min, 1->30min, 2->1h, 3->2h, 4->nunca
  },
  encerrarSessao:{
    type: Number,
    required: false //0->1min, 1->3min, 2->5min, 3->10min, 4->nunca
  },
  email:{
    type: String,
    required: false
  },
  email2:{
    type: String,
    required: false
  },
  email3:{
    type: String,
    required: false
  },
  identificacao:{
    type: String,
    required: false
  },
  dataRelatorio:{
    type: String,
    required: true
  }
})

mongoose.model("configuracoes", Configuracoes)
