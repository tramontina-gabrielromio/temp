const mongoose = require('mongoose')
const Schema = mongoose.Schema

const Usuario = new Schema({
  nome:{
    type: String,
    required: true
  },
  setor:{
    type: String,
    required: true
  },
  login:{
    type: String,
    required: true
  },
  senha:{
    type: String,
    required: true
  },
  rfid:{
    type: String,
    required: false
  },
  nivel:{
    type: Number,
    default: 0
  },
  gv1:{
    type: Boolean,
    default: 0
  },
  gv2:{
    type: Boolean,
    default: 0
  },
  gv3:{
    type: Boolean,
    default: 0
  },
  gv4:{
    type: Boolean,
    default: 0
  },
  gv5:{
    type: Boolean,
    default: 0
  },
  gv6:{
    type: Boolean,
    default: 0
  },
  gv7:{
    type: Boolean,
    default: 0
  },
  gv8:{
    type: Boolean,
    default: 0
  }
})

mongoose.model("usuarios", Usuario)
