import 'package:firebase_database/ui/firebase_animated_list.dart';
import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'firebase_options.dart';
import 'package:firebase_database/firebase_database.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp(
    options: DefaultFirebaseOptions.currentPlatform,
  );

  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  DatabaseReference dbref = FirebaseDatabase.instanceFor(
          app: Firebase.app(),
          databaseURL:
              'systemprojectgroup1-default-rtdb.europe-west1.firebasedatabase.app')
      .ref();

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'app',
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Inventory'),
        ),
        body: FirebaseAnimatedList(
          query: dbref,
          itemBuilder: (context, snapshot, animation, index) {
            int contents = snapshot.child('contents').value as int;
            int capacity = snapshot.child('capacity').value as int;
            var arduinoId = snapshot.child('arduinoId').value.toString();
            return Card(
              child: ListTile(
                title: Text(snapshot.child('shelfId').value.toString() +
                    ": " +
                    (contents / capacity * 100).round().toString() +
                    "%"),
                subtitle: Text('Current stock level: $contents/$capacity'),
                trailing: Text('Arduino ID: $arduinoId'),
              ),
            );
          },
        ),
      ),
    );
  }
}
