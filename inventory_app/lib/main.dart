import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'firebase_options.dart';
import 'package:cloud_firestore/cloud_firestore.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp(
    options: DefaultFirebaseOptions.currentPlatform,
  );

  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'app',
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Inventory'),
        ),
        body: Center(
          child: StreamBuilder(
            // need to handle loading
            stream:
                FirebaseFirestore.instance.collection('shelves').snapshots(),
            builder: (context, AsyncSnapshot<QuerySnapshot> snapshot) {
              if (!snapshot.hasData) {
                return Container();
              }
              return ListView(
                children: snapshot.data!.docs.map(
                  (e) {
                    return Card(
                      child: Padding(
                        padding: const EdgeInsets.all(30),
                        child: Column(
                          children: [
                            Text(e.id),
                            const SizedBox(height: 8),
                            Text('contents: ' + e['contents'].toString()),
                            const SizedBox(height: 8),
                            Text('capacity: ' + e['capacity'].toString()),
                          ],
                        ),
                      ),
                    );
                  },
                ).toList(),
              );
            },
          ),
        ),
      ),
    );
  }
}
