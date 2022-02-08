var SerialPort = require('serialport');

const { initializeApp, applicationDefault, cert } = require('firebase-admin/app');
const { getFirestore, Timestamp, FieldValue } = require('firebase-admin/firestore');
const serviceAccount = require('./firebase-admin-key.json');

initializeApp({
  credential: cert(serviceAccount)
});

const db = getFirestore();

const parsers = SerialPort.parsers;
const parser = new parsers.Readline({
    delimiter: '\r\n'
});

var port = new SerialPort('COM5',{ 
    baudRate: 9600,
    dataBits: 8,
    parity: 'none',
    stopBits: 1,
    flowControl: false
});

port.pipe(parser);

parser.on('data', function(data){
    console.log(data);
    setData(data);
});

async function setData(data) {
    await db.collection('test').doc('switch').set({
        reading: data
    });
}