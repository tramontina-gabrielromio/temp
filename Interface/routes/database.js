const express = require('express')
const router = express.Router()
const mongoose = require("mongoose")
require("../models/Ferramenta")
const Ferramenta = mongoose.model("ferramentas")
const novaFerramenta = new Array(100)

require("../models/Evento")
const Evento = mongoose.model("eventos")

//Ferramentas gaveta 1
novaFerramenta[1] = {
  descricao: 'Alicate Corte Rente Diagonal 6"',
  referencia: '44002126',
  gaveta: 1,
  posicao: 1,
  estado: 1,
  desconhecido: 0}
novaFerramenta[2] = {
  descricao: 'Alicate Corte Diagonal Isolado 1.000 V 6"',
  referencia: '44003106',
  gaveta: 1,
  posicao: 2,
  estado: 1,
  desconhecido: 0}
novaFerramenta[3] = {
  descricao: 'Alicate para Eletrônica Corte Diagonal 4"',
  referencia: '44070101',
  gaveta: 1,
  posicao: 3,
  estado: 1,
  desconhecido: 0}
novaFerramenta[4] = {
  descricao: 'Alicate para Eletrônica Meia Cana 5"',
  referencia: '44071101',
  gaveta: 1,
  posicao: 4,
  estado: 1,
  desconhecido: 0}
novaFerramenta[5] = {
  descricao: 'Alicate para Eletrônica Bico Longo 6"',
  referencia: '44074101',
  gaveta: 1,
  posicao: 5,
  estado: 1,
  desconhecido: 0}
novaFerramenta[6] = {
  descricao: 'Alicate Meia Cana - Bico Longo Curvo Isolado 1.000 V 8"',
  referencia: '44004181',
  gaveta: 1,
  posicao: 6,
  estado: 1,
  desconhecido: 0}
novaFerramenta[7] = {
  descricao: 'Alicate Meia Cana - Bico Longo Reto Isolado 1.000 V 8"',
  referencia: '44004183',
  gaveta: 1,
  posicao: 7,
  estado: 1,
  desconhecido: 0}
novaFerramenta[8] = {
  descricao: 'Alicate para Anéis Internos Reto 7"',
  referencia: '44008107',
  gaveta: 1,
  posicao: 8,
  estado: 1,
  desconhecido: 0}
novaFerramenta[9] = {
  descricao: 'Alicate para Anéis Internos Curvo 7"',
  referencia: '44009107',
  gaveta: 1,
  posicao: 9,
  estado: 1,
  desconhecido: 0}
novaFerramenta[10] = {
  descricao: 'Alicate para Anéis Externos Reto 7"',
  referencia: '44010107',
  gaveta: 1,
  posicao: 10,
  estado: 1,
  desconhecido: 0}
novaFerramenta[11] = {
  descricao: 'Alicate para Anéis Externos Curvo 7"',
  referencia: '44011107',
  gaveta: 1,
  posicao: 11,
  estado: 1,
  desconhecido: 0}

//Ferramentas gaveta 2
novaFerramenta[12] = {
  descricao: 'Alicate para Arame de Segurança 9"',
  referencia: '44060101',
  gaveta: 2,
  posicao: 1,
  estado: 1,
  desconhecido: 0}
novaFerramenta[13] = {
  descricao: 'Tesoura de Aviação Corte Direito',
  referencia: '44023103',
  gaveta: 2,
  posicao: 2,
  estado: 1,
  desconhecido: 0}
novaFerramenta[14] = {
  descricao: 'Tesoura de Aviação Corte Esquerdo',
  referencia: '44023102',
  gaveta: 2,
  posicao: 3,
  estado: 1,
  desconhecido: 0}
novaFerramenta[15] = {
  descricao: 'Tesoura de Aviação Corte Reto',
  referencia: '44023101',
  gaveta: 2,
  posicao: 4,
  estado: 1,
  desconhecido: 0}
  
//Ferramentas gaveta 3
novaFerramenta[16] = {
  descricao: 'Chave de Fenda Ponta Chata 6x100 mm (1/4x4")',
  referencia: '44130030',
  gaveta: 3,
  posicao: 1,
  estado: 1,
  desconhecido: 0}
novaFerramenta[17] = {
  descricao: 'Chave de Fenda Toco Ponta Cruzada 5x38 mm (3/16x1.1/2")',
  referencia: '44137001',
  gaveta: 3,
  posicao: 2,
  estado: 1,
  desconhecido: 0}
novaFerramenta[18] = {
  descricao: 'Chave de Fenda Ponta Chata 5x38 mm (3/16x1.1/2")',
  referencia: '44132001',
  gaveta: 3,
  posicao: 3,
  estado: 1,
  desconhecido: 0}
novaFerramenta[19] = {
  descricao: 'Chave de Fenda Ponta Cruzada 5x100 mm (3/16x4")',
  referencia: '44135021',
  gaveta: 3,
  posicao: 4,
  estado: 1,
  desconhecido: 0}
novaFerramenta[20] = {
  descricao: 'Chave de Fenda Ponta Cruzada 6x150 mm (1/4x6")',
  referencia: '44135031',
  gaveta: 3,
  posicao: 5,
  estado: 1,
  desconhecido: 0}
novaFerramenta[21] = {
  descricao: 'Chave de Fenda Ponta Cruzada 6x250 mm (1/4x10")',
  referencia: '44135033',
  gaveta: 3,
  posicao: 6,
  estado: 1,
  desconhecido: 0}
novaFerramenta[22] = {
  descricao: 'Chave de Fenda Ponta Cruzada 3 mm',
  referencia: '44052905',
  gaveta: 3,
  posicao: 7,
  estado: 1,
  desconhecido: 0}
novaFerramenta[23] = {
  descricao: 'Chave de Fenda Ponta Chata 3 mm',
  referencia: '44052904',
  gaveta: 3,
  posicao: 8,
  estado: 1,
  desconhecido: 0}
novaFerramenta[24] = {
  descricao: 'Chave de Fenda Ponta Cruzada 2,5 mm',
  referencia: '44052906',
  gaveta: 3,
  posicao: 9,
  estado: 1,
  desconhecido: 0}  
novaFerramenta[25] = {
  descricao: 'Chave de Fenda Ponta Chata 2,5 mm',
  referencia: '44052903',
  gaveta: 3,
  posicao: 10,
  estado: 1,
  desconhecido: 0}
novaFerramenta[26] = {
  descricao: 'Chave de Fenda Ponta Trafix 8',
  referencia: '44052901',
  gaveta: 3,
  posicao: 11,
  estado: 1,
  desconhecido: 0}
novaFerramenta[27] = {
  descricao: 'Chave de Fenda Ponta Trafix 10',
  referencia: '44052902',
  gaveta: 3,
  posicao: 12,
  estado: 1,
  desconhecido: 0}
  
//Ferramentas gaveta 4
novaFerramenta[28] = {
  descricao: 'Chave Meia-Lua 9/16x5/8"',
  referencia: '44676103',
  gaveta: 4,
  posicao: 1,
  estado: 1,
  desconhecido: 0}
novaFerramenta[29] = {
  descricao: 'Chave Poligonal Aberta 7/16x1/2"',
  referencia: '44636104',
  gaveta: 4,
  posicao: 2,
  estado: 1,
  desconhecido: 0}
novaFerramenta[30] = {
  descricao: 'Chave Poligonal Aberta 3/8x7/16" ',
  referencia: '44636103',
  gaveta: 4,
  posicao: 3,
  estado: 1,
  desconhecido: 0}  
novaFerramenta[31] = {
  descricao: 'Chave Estrela com Catraca 12x13 mm',
  referencia: '44634104',
  gaveta: 4,
  posicao: 4,
  estado: 1,
  desconhecido: 0}
novaFerramenta[32] = {
  descricao: 'Chave Combinada 7/16"',
  referencia: '44670103',
  gaveta: 4,
  posicao: 5,
  estado: 1,
  desconhecido: 0}
novaFerramenta[33] = {
  descricao: 'Chave Combinada 1/2"',
  referencia: '44670104',
  gaveta: 4,
  posicao: 6,
  estado: 1,
  desconhecido: 0}
novaFerramenta[34] = {
  descricao: 'Chave Combinada 9/16"',
  referencia: '44670105',
  gaveta: 4,
  posicao: 7,
  estado: 1,
  desconhecido: 0}
novaFerramenta[35] = {
  descricao: 'Chave Combinada 5/8"',
  referencia: '44670106',
  gaveta: 4,
  posicao: 8,
  estado: 1,
  desconhecido: 0}
novaFerramenta[36] = {
  descricao: 'Chave Combinada 11/16"',
  referencia: '44670107',
  gaveta: 4,
  posicao: 9,
  estado: 1,
  desconhecido: 0}
novaFerramenta[37] = {
  descricao: 'Chave Combinada 7/8"',
  referencia: '44670111',
  gaveta: 4,
  posicao: 10,
  estado: 1,
  desconhecido: 0}
  
//Ferramentas gaveta 5
novaFerramenta[38] = {
  descricao: 'Chave Fixa 1/2x9/16"',
  referencia: '44620102',
  gaveta: 5,
  posicao: 1,
  estado: 1,
  desconhecido: 0}
novaFerramenta[39] = {
  descricao: 'Chave Fixa 1/4x5/16"',
  referencia: '44620100',
  gaveta: 5,
  posicao: 2,
  estado: 1,
  desconhecido: 0}
novaFerramenta[40] = {
  descricao: 'Chave Fixa 5/8x11/16"',
  referencia: '44620103',
  gaveta: 5,
  posicao: 3,
  estado: 1,
  desconhecido: 0}
novaFerramenta[41] = {
  descricao: 'Chave com Catraca 4 Bocas 9/16"x5/8"x11/16"x3/4"',
  referencia: '44641103',
  gaveta: 5,
  posicao: 4,
  estado: 1,
  desconhecido: 0}
novaFerramenta[42] = {
  descricao: 'Chave com Catraca 4 Bocas 5/16"x3/8"x7/16"x1/2"',
  referencia: '44641101',
  gaveta: 5,
  posicao: 5,
  estado: 1,
  desconhecido: 0}  
novaFerramenta[43] = {
  descricao: 'Chave com Catraca 4 Bocas 8x9x10x11 mm',
  referencia: '44637101',
  gaveta: 5,
  posicao: 6,
  estado: 1,
  desconhecido: 0}
novaFerramenta[44] = {
  descricao: 'Chave com Catraca 4 Bocas 12x13x14x15 mm',
  referencia: '44637103',
  gaveta: 5,
  posicao: 7,
  estado: 1,
  desconhecido: 0}
novaFerramenta[45] = {
  descricao: 'Chave com Catraca 4 Bocas 13x15x17x19 mm',
  referencia: '44637104',
  gaveta: 5,
  posicao: 8,
  estado: 1,
  desconhecido: 0}
novaFerramenta[46] = {
  descricao: 'Chave Combinada com Catraca 17 mm',
  referencia: '44681117',
  gaveta: 5,
  posicao: 9,
  estado: 1,
  desconhecido: 0}
novaFerramenta[47] = {
  descricao: 'Chave Combinada com Catraca 19 mm',
  referencia: '44652119',
  gaveta: 5,
  posicao: 10,
  estado: 1,
  desconhecido: 0}

//Ferramentas gaveta 6
novaFerramenta[48] = {
  descricao: 'Soquete Ponta Hexagonal 7 mm - Encaixe 3/8"',
  referencia: '44812107',
  gaveta: 6,
  posicao: 1,
  estado: 1,
  desconhecido: 0}
novaFerramenta[49] = {
  descricao: 'Soquete Ponta Hexagonal 5 mm - Encaixe 3/8"',
  referencia: '44812105',
  gaveta: 6,
  posicao: 2,
  estado: 1,
  desconhecido: 0}
novaFerramenta[50] = {
  descricao: 'Soquete Ponta Trafix T45 - Encaixe 1/2"',
  referencia: '44844145',
  gaveta: 6,
  posicao: 3,
  estado: 1,
  desconhecido: 0}
novaFerramenta[51] = {
  descricao: 'Soquete Ponta Hexagonal 12 mm - Encaixe 1/2"',
  referencia: '44845112',
  gaveta: 6,
  posicao: 4,
  estado: 1,
  desconhecido: 0}
novaFerramenta[52] = {
  descricao: 'Soquete Ponta Chata Tamanho 8 - Encaixe 1/2"',
  referencia: '44842108',
  gaveta: 6,
  posicao: 5,
  estado: 1,
  desconhecido: 0}
novaFerramenta[53] = {
  descricao: 'Soquete Ponta Cruzada NR3 - Encaixe 1/2"',
  referencia: '44843103',
  gaveta: 6,
  posicao: 6,
  estado: 1,
  desconhecido: 0}
novaFerramenta[54] = {
  descricao: 'Soquete Estriado Longo 7/8" - Encaixe 3/8"',
  referencia: '44869109',
  gaveta: 6,
  posicao: 7,
  estado: 1,
  desconhecido: 0}
novaFerramenta[55] = {
  descricao: 'Soquete Xtractor Plus 19 mm - Encaixe 1/2"',
  referencia: '44851119',
  gaveta: 6,
  posicao: 8,
  estado: 1,
  desconhecido: 0}
novaFerramenta[56] = {
  descricao: 'Soquete Xtractor Plus 17 mm - Encaixe 1/2"',
  referencia: '44851117',
  gaveta: 6,
  posicao: 9,
  estado: 1,
  desconhecido: 0}
novaFerramenta[57] = {
  descricao: 'Soquete Xtractor Plus 13 mm - Encaixe 1/2"',
  referencia: '44851113',
  gaveta: 6,
  posicao: 10,
  estado: 1,
  desconhecido: 0}  
novaFerramenta[58] = {
  descricao: 'Soquete Estriado 3/8" - Encaixe 1/4"',
  referencia: '44811107',
  gaveta: 6,
  posicao: 11,
  estado: 1,
  desconhecido: 0}
novaFerramenta[59] = {
  descricao: 'Catraca 8" - Encaixe 3/8"',
  referencia: '44826108',
  gaveta: 6,
  posicao: 12,
  estado: 1,
  desconhecido: 0}
novaFerramenta[60] = {
  descricao: 'Adaptador 1/4" x 3/8"',
  referencia: '44801101',
  gaveta: 6,
  posicao: 13,
  estado: 1,
  desconhecido: 0}
novaFerramenta[61] = {
  descricao: 'Catraca 6" - Encaixe 1/4"',
  referencia: '44804100',
  gaveta: 6,
  posicao: 14,
  estado: 1,
  desconhecido: 0}
novaFerramenta[62] = {
  descricao: 'Extensão 4" - Encaixe 1/4"',
  referencia: '44804104',
  gaveta: 6,
  posicao: 15,
  estado: 1,
  desconhecido: 0}  
novaFerramenta[63] = {
  descricao: 'Junta Universal - Encaixe 1/2"',
  referencia: '44838101',
  gaveta: 6,
  posicao: 16,
  estado: 1,
  desconhecido: 0}
novaFerramenta[64] = {
  descricao: 'Catraca - Encaixe 1/2"',
  referencia: '44841101',
  gaveta: 6,
  posicao: 17,
  estado: 1,
  desconhecido: 0}  
novaFerramenta[65] = {
  descricao: 'Catraca Telescópica Cabeça Flexível - Encaixe 1/2"',
  referencia: '44841102',
  gaveta: 6,
  posicao: 18,
  estado: 1,
  desconhecido: 0}
novaFerramenta[66] = {
  descricao: 'Catraca Telescópica - Encaixe 1/2"',
  referencia: '44841112',
  gaveta: 6,
  posicao: 19,
  estado: 1,
  desconhecido: 0}

//Ferramentas gaveta 7
novaFerramenta[67] = {
  descricao: 'Lanterna de LED Recarregável com Gancho Magnético',
  referencia: '44550303',
  gaveta: 7,
  posicao: 1,
  estado: 1,
  desconhecido: 0}
novaFerramenta[68] = {
  descricao: 'Jogo de Chaves Hexagonais com Armação 7 Peças',
  referencia: '44401207',
  gaveta: 7,
  posicao: 2,
  estado: 1,
  desconhecido: 0}
novaFerramenta[69] = {
  descricao: 'Trena Profissional Tramontina com Ponta Magnética 5 m',
  referencia: '43158305',
  gaveta: 7,
  posicao: 3,
  estado: 1,
  desconhecido: 0}
novaFerramenta[70] = {
  descricao: 'Riscador Tipo Caneta',
  referencia: '44095501',
  gaveta: 7,
  posicao: 4,
  estado: 1,
  desconhecido: 0}
novaFerramenta[71] = {
  descricao: 'Lanterna de LED Recarregável com Base Magnética',
  referencia: '44550302',
  gaveta: 7,
  posicao: 5,
  estado: 1,
  desconhecido: 0}
novaFerramenta[72] = {
  descricao: 'Calculadora',
  referencia: '00000001',
  gaveta: 7,
  posicao: 6,
  estado: 1,
  desconhecido: 0}
novaFerramenta[73] = {
  descricao: 'Mini Corta-Tubos - PVC e Cobre',
  referencia: '44036030',
  gaveta: 7,
  posicao: 7,
  estado: 1,
  desconhecido: 0}
novaFerramenta[74] = {
  descricao: 'Alicate Desencapador de Fios Automático 8"',
  referencia: '44051108',
  gaveta: 7,
  posicao: 8,
  estado: 1,
  desconhecido: 0}  
novaFerramenta[75] = {
  descricao: 'Faca Industrial',
  referencia: '44033501',
  gaveta: 7,
  posicao: 9,
  estado: 1,
  desconhecido: 0}

//Ferramentas gaveta 8
novaFerramenta[76] = {
  descricao: 'Extensão Telescópica com Espelho Articulado 360°',
  referencia: '44099150',
  gaveta: 8,
  posicao: 1,
  estado: 1,
  desconhecido: 0}
novaFerramenta[77] = {
  descricao: 'Martelo sem Retrocesso 500 g',
  referencia: '40673037',
  gaveta: 8,
  posicao: 2,
  estado: 1,
  desconhecido: 0}
novaFerramenta[78] = {
  descricao: 'Cabeça Intercambiável Chave Estrela 19 mm - Encaixe 9x12',
  referencia: '44512019',
  gaveta: 8,
  posicao: 3,
  estado: 1,
  desconhecido: 0}
novaFerramenta[79] = {
  descricao: 'Cabeça Intercambiável Chave Fixa 19 mm - Encaixe 9x12',
  referencia: '44511019',
  gaveta: 8,
  posicao: 4,
  estado: 1,
  desconhecido: 0}
novaFerramenta[80] = {
  descricao: 'Cabeça Intercambiável Chave Catraca 3/8" - Encaixe 9x12',
  referencia: '44515001',
  gaveta: 8,
  posicao: 5,
  estado: 1,
  desconhecido: 0}
novaFerramenta[81] = {
  descricao: 'Cabeça Intercambiável Chave Poligonal Aberta 19 mm - Encaixe 9x12',
  referencia: '44517019',
  gaveta: 8,
  posicao: 6,
  estado: 1,
  desconhecido: 0}
novaFerramenta[82] = {
  descricao: 'Torquímetro de Estalo Intercambiável 20-100 N.m - Encaixe 9x12',
  referencia: '44510202',
  gaveta: 8,
  posicao: 7,
  estado: 1,
  documentacao: '2155472300069',
  desconhecido: 0}
novaFerramenta[83] = {
  descricao: 'Martelo de Borda em Poliuretano 30 mm',
  referencia: '40670032',
  gaveta: 8,
  posicao: 8,
  estado: 1,
  desconhecido: 0}

for (var i=1; i <= 83; i++){
    new Ferramenta(novaFerramenta[i]).save();
}

/*const novoEvento = {
    referencia: '44135/041',
    descricao: 'Chave phillips 8x150mm',
    gaveta: 2,
    posicao: 1,
    dataRetirada: '25/08/2021',
    horaRetirada: '12:00',
    dataDevolucao: '25/08/2021',
    horaDevolucao: '14:00',
    usuario: 'Gabriel',
    setor: 'Automacao',
    devolucao: 1}*/

//new Evento(novoEvento).save();

module.exports = router