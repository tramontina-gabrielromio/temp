const mongoose = require('mongoose')
const Schema = mongoose.Schema

const DadosTeste = new Schema({
  data:{
    type: String,
    required: true
  },
  gaveta:{
    type: Number,
    required: true
  },
  tempo:{
    type: String,
    required: true
  },
  contagemCiclos:{
    type: Number,
    required: true
  },
  tipo:{
    type: Number, //0-Teste gaveta especifica / 1-Teste todas as gavetas
    required: true
  },
  finalizado:{
    type: Number, //0-Teste falhou / 1-Teste concluído com sucesso
    required: true
  }
})

mongoose.model("dadosTestes", DadosTeste)
