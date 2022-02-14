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

var port = new SerialPort('COM5', {
    baudRate: 9600
});

port.pipe(parser);

parser.on('data', function (data) {
    console.log(data);
    var obj = JSON.parse(data);
    var shelves = obj.shelves;
    shelves.forEach(function (entry) {
        setShelf(entry, obj.arduinoId);
    })
});

async function setShelf(shelf, arduinoId) {
    await db.collection('shelves').doc(shelf.shelfId).set({
        arduinoId: arduinoId,
        contents: shelf.contents,
        capacity: shelf.capacity
    });
}