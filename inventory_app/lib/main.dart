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

  runApp(const MaterialApp(title: 'RobotInventory', home: MyApp()));
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
              // Todo: Search db for product with this code
              barcodeScanRes = await FlutterBarcodeScanner.scanBarcode(
                  "#ff6666", "Cancel", false, ScanMode.BARCODE);
              await listref.set(
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
                  });
            },
            icon: const Icon(
              Icons.qr_code_scanner_rounded,
            ),
          ),
        ],
      ),
      body: FirebaseAnimatedList(
        query: listref,
        itemBuilder: (context, snapshot, animation, index) {
          int contents = snapshot.child('locs/contents').value as int;
          int capacity = snapshot.child('locs/capacity').value as int;
          var arduinoId = snapshot.child('locs/arduinoId').value.toString();
          return Card(
            child: ListTile(
              title: Text(snapshot.child('locs/shelfId').value.toString() +
                  ": " +
                  (contents / capacity * 100).round().toString() +
                  "%"),
              subtitle: Text('Current stock level: $contents/$capacity'),
              trailing: Text('Arduino ID: $arduinoId'),
            ),
          );
        },
      ),
    );
  }
}
