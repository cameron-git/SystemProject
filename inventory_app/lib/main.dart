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
            var sn = snapshot.value as int;
            return ListTile(
              title: Text(sn.toString()),
            );
          },
        ),
      ),
    );
  }
}
