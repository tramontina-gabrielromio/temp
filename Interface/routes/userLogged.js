var userData = {nome: '', login: '', setor: '', gv1: 0, gv2: 0, gv3: 0, gv4: 0, gv5: 0, gv6: 0, gv7: 0, gv8: 0,};

var user = function(op, data){

  if(op=="insertUserdata"){
    userData.nome = data.nome;
    userData.login = data.login;
    userData.setor = data.setor;
    userData.gv1 = data.gv1;
    userData.gv2 = data.gv2;
    userData.gv3 = data.gv3;
    userData.gv4 = data.gv4;
    userData.gv5 = data.gv5;
    userData.gv6 = data.gv6;
    userData.gv7 = data.gv7;
    userData.gv8 = data.gv8;
    return userData;
  }
  else if(op=="showUserdata"){
    return userData
  }
  else if(op=="resetUserdata"){
    userData.nome = '';
    userData.login = '';
    userData.setor = '';
    userData.gv1 = 0;
    userData.gv2 = 0;
    userData.gv3 = 0;
    userData.gv4 = 0;
    userData.gv5 = 0;
    userData.gv6 = 0;
    userData.gv7 = 0;
    userData.gv8 = 0;
    return userData
  }
}

module.exports = user;
