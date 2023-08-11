module.exports = {
  admin: function(req, res, next){
    if(req.isAuthenticated() && req.user.nivel >= 1){
      return next();
    }
    req.flash("error_msg", "Você não tem permissão para isso!")
    res.redirect("/")
  }
}
