const mongoose = require('mongoose')
const Schema = mongoose.Schema

const Ferramenta = new Schema({
  descricao:{
    type: String,
    required: true
  },
  referencia:{
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
  estado:{
    type: Number,
    required: true
  },
  desconhecido:{
    type: Number,
    required: true
  },
  documentacao:{
    type: String,
    required: false
  }
})

mongoose.model("ferramentas", Ferramenta)
