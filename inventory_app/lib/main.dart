import 'package:firebase_database/ui/firebase_animated_list.dart';
import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:flutter_barcode_scanner/flutter_barcode_scanner.dart';
import 'firebase_options.dart';
import 'package:firebase_database/firebase_database.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp(
    options: DefaultFirebaseOptions.currentPlatform,
  );

  runApp(const MaterialApp(
    title: 'RobotInventory',
    home: MyApp(),
    debugShowCheckedModeBanner: false,
  ));
}

class MyApp extends StatefulWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  DatabaseReference listref = FirebaseDatabase.instance.ref();
  String barcodeScanRes = '';

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Inventory'),
        actions: [
          IconButton(
            onPressed: () async {
              await listref.update(
                {
                  'barcode': 'stop',
                },
              );
            },
            icon: const Icon(Icons.not_interested),
          ),
          IconButton(
            onPressed: () async {
              await listref.update(
                {
                  'barcode': '0000',
                },
              );
            },
            icon: const Icon(Icons.warehouse),
          ),
          IconButton(
            onPressed: () async {
              // Todo: Search db for product with this code
              barcodeScanRes = await FlutterBarcodeScanner.scanBarcode(
                  "#ff6666", "Cancel", false, ScanMode.BARCODE);
              await listref.update(
                {
                  'barcode': barcodeScanRes,
                },
              );
              showDialog(
                context: context,
                builder: (context) {
                  return AlertDialog(
                    title: Text('Sending robot to $barcodeScanRes'),
                    actions: [
                      TextButton(
                        child: const Text('Ok'),
                        onPressed: () {
                          Navigator.of(context).pop();
                        },
                      ),
                    ],
                  );
                },
              );
            },
            icon: const Icon(
              Icons.qr_code_scanner_rounded,
            ),
          ),
        ],
      ),
      body: FirebaseAnimatedList(
        query: listref.child('locs'),
        itemBuilder: (context, snapshot, animation, index) {
          String barcode = snapshot.child('barcode').value as String;
          int contents = snapshot.child('contents').value as int;
          int capacity = snapshot.child('capacity').value as int;
          String type = snapshot.child('type').value as String;
          String arduinoId = snapshot.child('arduinoId').value as String;
          return InkWell(
            child: Card(
              child: ListTile(
                title: Row(
                  children: [
                    Text(
                      snapshot.child('shelfId').value.toString() +
                          " (" +
                          type +
                          ") : ",
                    ),
                    (type == "ldr")
                        ? Text(
                            (contents / capacity * 100).round().toString() +
                                "%",
                          )
                        : Text(
                            contents.toString(),
                          ),
                  ],
                ),
                subtitle: (type == "ldr")
                    ? Text('Current stock level: $contents/$capacity')
                    : Text('Current stock level: $contents'),
                trailing: Text('Arduino ID: $arduinoId'),
              ),
            ),
            onTap: () async {
              await listref.update(
                {
                  'barcode': barcode,
                },
              );
            },
          );
        },
      ),
    );
  }
}
