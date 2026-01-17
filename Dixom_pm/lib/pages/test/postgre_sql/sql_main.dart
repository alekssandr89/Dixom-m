import 'package:dixom/pages/test/inherited/Inherited_notifier_finish.dart';
import 'package:flutter/material.dart';
import 'package:postgres/postgres.dart';

class SqlMain extends StatelessWidget {
  const SqlMain({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: SafeArea(
        child: SimpleCalcWidget(),
      ),
    );
  }
}

class SimpleCalcWidget extends StatelessWidget {
  SimpleCalcWidget({super.key});

  final _model = SimpleCalcWidgetModel();

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Padding(
        padding: const EdgeInsets.symmetric(horizontal: 30),
        child: ChangeNotifierProvider(
          model: _model,
          child: const Column(
            mainAxisSize: MainAxisSize.min,
            children: [
              FirstNumberWidget(),
              SizedBox(height: 10),
              SecondNumberWidget(),
              SizedBox(height: 10),
              SummButtonWidget(),
              SizedBox(height: 10),
            ],
          ),
        ),
      ),
    );
  }
}

class FirstNumberWidget extends StatelessWidget {
  const FirstNumberWidget({super.key});

  @override
  Widget build(BuildContext context) {
    return TextField(
      decoration: const InputDecoration(border: OutlineInputBorder()),
      onChanged: (String value) => ChangeNotifierProvider.read<SimpleCalcWidgetModel>(context)?.firstNumber = value,
    );
  }
}

class SecondNumberWidget extends StatelessWidget {
  const SecondNumberWidget({super.key});

  @override
  Widget build(BuildContext context) {
    return const TextField(
      decoration: InputDecoration(border: OutlineInputBorder()),
    );
  }
}

class SummButtonWidget extends StatelessWidget {
  const SummButtonWidget({super.key});

  @override
  Widget build(BuildContext context) {
    return ElevatedButton(
      onPressed: () {
        sqlMain();
      },
      child: const Text('Подключиться к серверу'),
    );
  }
}

void sqlMain() async {
  final conn = await Connection.open(
    Endpoint(
      host: '4ce38d9f2154.vps.myjino.ru',
      database: 'postgres',
      username: 'user',
      password: 'pass',
      port: 22,
    ),

    // The postgres server hosted locally doesn't have SSL by default. If you're
    // accessing a postgres server over the Internet, the server should support
    // SSL and you should swap out the mode with `SslMode.verifyFull`.
    settings: const ConnectionSettings(sslMode: SslMode.disable),
  );
  print('has connection!');

  /*
 

  // Simple query without results
  await conn.execute('CREATE TABLE IF NOT EXISTS a_table ('
      '  id TEXT NOT NULL, '
      '  totals INTEGER NOT NULL DEFAULT 0'
      ')');

  // simple query
  final result0 = await conn.execute("SELECT 'foo'");
  print(result0[0][0]); // first row, first column

  // Using prepared statements to supply values
  final result1 = await conn.execute(
    r'INSERT INTO a_table (id) VALUES ($1)',
    parameters: ['example row'],
  );
  print('Inserted ${result1.affectedRows} rows');

  // name parameter query
  final result2 = await conn.execute(
    Sql.named('SELECT * FROM a_table WHERE id=@id'),
    parameters: {'id': 'example row'},
  );
  print(result2.first.toColumnMap());

  // transaction
  await conn.runTx((s) async {
    final rs = await s.execute('SELECT count(*) FROM a_table');
    await s.execute(
      r'UPDATE a_table SET totals=$1 WHERE id=$2',
      parameters: [rs[0][0], 'xyz'],
    );
  });

  // prepared statement
  final statement = await conn.prepare(Sql("SELECT 'foo';"));
  final result3 = await statement.run([]);
  print(result3);
  await statement.dispose();

  // preared statement with types
  final anotherStatement = await conn.prepare(Sql(r'SELECT $1;', types: [Type.bigInteger]));
  final bound = anotherStatement.bind([1]);
  final subscription = bound.listen((row) {
    print('row: $row');
  });
  await subscription.asFuture();
  await subscription.cancel();
  print(await subscription.affectedRows);
  print(await subscription.schema);

  await conn.close();*/
}
