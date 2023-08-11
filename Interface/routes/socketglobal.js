const express = require('express')
const router = express.Router()
const mongoose = require('mongoose')
const path = require("path")
const { execFile } = require('child_process');
const {spawn} = require('child_process');


//Funcções sockets
const method = function (global) {
    //Socket.IO
    var teste = global.on('connection', function (socket) {
        console.log('Usuario global conectado');
    });
    console.log(teste)
    return router;
};

module.exports = method