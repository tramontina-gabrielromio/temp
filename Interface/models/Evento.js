const mongoose = require('mongoose')
const Schema = mongoose.Schema

const Evento = new Schema({
  referencia:{
    type: String,
    required: true
  },
  descricao:{
    type: String,
    required: true
  },
  gaveta:{
    type: Number,
    required: true
  },
  posicao:{
    type: Number,
    required: true
  },
  dataRetirada:{
    type: String,
    required: true
  },
  horaRetirada:{
    type: String,
    required: true
  },
  dataDevolucao:{
    type: String,
    required: true
  },
  horaDevolucao:{
    type: String,
    required: true
  },
  usuario:{
    type: String,
    required: true
  },
  setor:{
    type: String,
    required: true
  },
  devolucao:{
    type: Number,
    required: true
  },
  enviado:{
    type: Number,
    required: true
  }
})

mongoose.model("eventos", Evento)