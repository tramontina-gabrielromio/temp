var memoriaSenha=""
var memoriaTentativas=1

var senha = function(op, data){

  if(op=="insertKey"){
    memoriaSenha = memoriaSenha + data
    return memoriaSenha;
  }
  else if(op=="removeKey"){
    memoriaSenha = memoriaSenha.substring(0, (memoriaSenha.length - 1));
    return memoriaSenha;
  }
  else if(op=="showPass"){
    return memoriaSenha
  }
  else if(op=="resetPass"){
    memoriaSenha=""
    return memoriaSenha
  }
  else if(op=="incTentativas"){
    memoriaTentativas=memoriaTentativas+1
    return memoriaTentativas
  }
  else if(op=="showTentativas"){
    return memoriaTentativas
  }
  else if(op=="resetTentativas"){
    memoriaTentativas=1
    return 1
  }
}

module.exports = senha;
