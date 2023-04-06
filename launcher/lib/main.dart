import 'dart:async';
import 'dart:convert';
import 'dart:io';

import 'package:carousel_slider/carousel_slider.dart';
import 'package:dio/dio.dart';
import 'package:file_picker/file_picker.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_easyloading/flutter_easyloading.dart';
import 'package:logger/logger.dart';
import 'package:process_run/shell.dart';
import 'package:url_launcher/url_launcher.dart';
import 'package:version/version.dart';
import 'package:window_manager/window_manager.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();

  await windowManager.ensureInitialized();
  const WindowOptions windowOptions = WindowOptions(
    size: Size(1024, 768),
    minimumSize: Size(1024, 768),
    maximumSize: Size(1024, 768),
    center: true,
    backgroundColor: Colors.transparent,
    skipTaskbar: false,
    titleBarStyle: TitleBarStyle.hidden,
  );
  windowManager.waitUntilReadyToShow(windowOptions, () async {
    await windowManager.show();
    await windowManager.focus();
  });

  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: const MyHomePage(),
      builder: EasyLoading.init(),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key});

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage>
    with WindowListener, SingleTickerProviderStateMixin {
  var logger = Logger(
    printer: PrettyPrinter(
      colors: true,
      printEmojis: false,
      printTime: true,
    ),
  );

  var btnDisable = true;
  var exePath = Directory.current.path;
  var remoteUrl = '';
  var remember = false;
  var autoLogin = false;
  var logined = false;
  Map<String, dynamic> version = {
    'app': '2023.4.001',
    'resource': '2023.02.019',
  };
  Map<String, dynamic> remoteVersion = {
    'app': {
      'version': '0.0.0',
      'force': 0,
      'link': '',
    },
    'resource': {
      'version': '0.0.0',
      'force': 0,
      'link': '',
    },
  };
  var user = {
    "familyname": "",
    "totalPlayTime": 0,
    "membershipType": 0,
  };

  Dio dio = Dio(
    BaseOptions(
      connectTimeout: const Duration(seconds: 5),
      receiveTimeout: const Duration(seconds: 5),
    ),
  );

  List<Map<String, dynamic>> slide = [];
  List<Map<String, dynamic>> news = [];

  String status = "正在检查更新...";

  late TextEditingController usernameController;
  late TextEditingController passwordController;

  late TextEditingController clientPathController;

  late TabController tabController;
  List<Tab> myTabs = <Tab>[
    const Tab(
      child: Text(
        "登录器",
        style: TextStyle(fontSize: 24, fontFamily: '微软雅黑'),
      ),
    ),
    // const Tab(
    //   child: Text(
    //     "游戏",
    //     style: TextStyle(fontSize: 24, fontFamily: '微软雅黑'),
    //   ),
    // ),
    // const Tab(
    //   child: Text(
    //     "其他",
    //     style: TextStyle(fontSize: 24, fontFamily: '微软雅黑'),
    //   ),
    // ),
  ];

  @override
  void initState() {
    usernameController = TextEditingController();
    passwordController = TextEditingController();
    clientPathController = TextEditingController();
    tabController = TabController(vsync: this, length: myTabs.length);
    windowManager.addListener(this);
    readConfig();
    init();
    super.initState();
  }

  @override
  void dispose() {
    windowManager.removeListener(this);
    usernameController.dispose();
    passwordController.dispose();
    tabController.dispose();
    super.dispose();
  }

  @override
  void onWindowFocus() {
    setState(() {});
  }

  @override
  void onWindowClose() {
    writeConfig();
  }

  void init() async {
    regProtocol();
    await update();
    await getSlide();
    await getNews();

    var shell = Shell(
      throwOnError: false,
      workingDirectory: exePath,
    );
    await shell.run('patch -u');

    exit(0);
  }

  Future<void> getSlide() async {
    try {
      Response resp = await dio.post("$remoteUrl/api/slide/info",
          data: {
            "curPage": 0,
            "maxPage": 5,
            "sortBy": 'create_date',
            "descending": true,
          },
          options: Options(contentType: "application/json"));

      if (resp.data["status"] == 0) {
        EasyLoading.showError(resp.data["msg"]);
        return;
      }

      for (var element in resp.data["msg"]) {
        setState(() {
          slide.add(element);
        });
      }
    } on DioError catch (e) {
      if (e.response != null) {
        logger.i(e.response!.data);
        logger.i(e.response!.headers);
        logger.i(e.response!.requestOptions);
      } else {
        logger.i(e.requestOptions);
        logger.i(e.message);
      }

      setState(() {
        status = '网络连接失败';
      });
    }
  }

  Future<void> getNews() async {
    try {
      Response resp = await dio.get("$remoteUrl/api/news/latest",
          options: Options(contentType: "application/json"));

      if (resp.data["status"] == 0) {
        EasyLoading.showError(resp.data["msg"]);
        return;
      }

      for (var element in resp.data["msg"]) {
        if (int.tryParse(element['delete'].toString()) == 1) continue;

        setState(() {
          news.add(element);
        });
      }
    } on DioError catch (e) {
      if (e.response != null) {
        logger.i(e.response!.data);
        logger.i(e.response!.headers);
        logger.i(e.response!.requestOptions);
      } else {
        logger.i(e.requestOptions);
        logger.i(e.message);
      }

      setState(() {
        status = '网络连接失败';
      });
    }
  }

  void regProtocol() {
    // var hKey = calloc<HANDLE>();

    // try {
    //   if (RegCreateKey(
    //           HKEY_CLASSES_ROOT, "BDOLauncher".toNativeUtf16(), hKey) !=
    //       ERROR_SUCCESS) {
    //     logger.i("Can't create registry key 'BDOLauncher'");
    //   }

    //   var reg = "\"${Directory.current.path}\\launcher.exe\"".toNativeUtf16();
    //   if (RegSetKeyValue(hKey.value, "".toNativeUtf16(),
    //           "URL Protocol".toNativeUtf16(), REG_SZ, reg, reg.length * 2) !=
    //       ERROR_SUCCESS) {
    //     logger.i("Can't set registry key 'URL Protocol'");
    //   }

    //   reg = "BOO Launcher Protocol".toNativeUtf16();
    //   if (RegSetValue(
    //           hKey.value, "".toNativeUtf16(), REG_SZ, reg, reg.length) !=
    //       ERROR_SUCCESS) {
    //     logger.i("Can't set registry key 'BOO Launcher Protocol'");
    //   }

    //   RegCloseKey(hKey.value);
    //   free(hKey);

    //   hKey = calloc<HANDLE>();

    //   if (RegCreateKey(HKEY_CLASSES_ROOT,
    //           "BDOLauncher\\DefaultIcon".toNativeUtf16(), hKey) !=
    //       ERROR_SUCCESS) {
    //     logger.i("Can't create registry key 'BDOLauncher\\DefaultIcon'");
    //   }

    //   reg = "\"${Directory.current.path}\\launcher.exe\",0".toNativeUtf16();
    //   if (RegSetValue(
    //           hKey.value, "".toNativeUtf16(), REG_SZ, reg, reg.length) !=
    //       ERROR_SUCCESS) {
    //     logger.i("Can't set registry key 'BOO Launcher Protocol'");
    //   }

    //   RegCloseKey(hKey.value);
    //   free(hKey);

    //   hKey = calloc<HANDLE>();

    //   if (RegCreateKey(HKEY_CLASSES_ROOT,
    //           "BDOLauncher\\shell\\open\\command".toNativeUtf16(), hKey) !=
    //       ERROR_SUCCESS) {
    //     logger.i("Can't create registry key 'BDOLauncher\\DefaultIcon'");
    //   }

    //   reg =
    //       "\"${Directory.current.path}\\launcher.exe\" \"%1\"".toNativeUtf16();
    //   if (RegSetValue(
    //           hKey.value, "".toNativeUtf16(), REG_SZ, reg, reg.length) !=
    //       ERROR_SUCCESS) {
    //     logger.i("Can't set registry key 'BOO Launcher Protocol'");
    //   }
    // } finally {
    //   free(hKey);
    // }
  }

  void runGame() async {
    EasyLoading.show(status: "正在启动游戏...");

    var shell = Shell(
      throwOnError: false,
      workingDirectory: '${clientPathController.text}\\bin64',
    );
    final r = await shell.run(
        'BlackDesert64.exe ${usernameController.text},${passwordController.text}');

    if (r.last.exitCode <= 0) {
      EasyLoading.dismiss();
      EasyLoading.showError('启动游戏失败');
    } else {
      EasyLoading.dismiss();
    }
  }

  Future<void> onLogin() async {
    try {
      EasyLoading.show(status: "正在登陆...");

      Response resp = await dio.post(
        "$remoteUrl/api/user/signin",
        data: {
          "username": usernameController.text,
          "password": passwordController.text
        },
        options: Options(contentType: "application/json"),
      );

      EasyLoading.dismiss();

      if (resp.data["status"] == 0) {
        EasyLoading.showError(resp.data["msg"]);
        return;
      }

      setState(() {
        user = {
          "familyname": resp.data["msg"]['userNickname'],
          "totalPlayTime": resp.data["msg"]['totalPlayTime'],
          "membershipType": resp.data["msg"]['membershipType'],
        };

        logined = true;
      });
    } on DioError catch (e) {
      if (e.response != null) {
        logger.i(e.response!.data);
        logger.i(e.response!.headers);
        logger.i(e.response!.requestOptions);
      } else {
        logger.i(e.requestOptions);
        logger.i(e.message);
      }

      setState(() {
        status = '网络连接失败';
      });
    }
  }

  void readConfig() async {
    File config = File('config.json');

    if (!config.existsSync()) {
      config.createSync();
      config.writeAsStringSync(
        jsonEncode({
          "url": "http://1.13.22.82:51530",
          "username": '',
          "password": '',
          "remember": false,
          "autoLogin": false,
          "client path": '',
          "version": version
        }),
      );
    }

    var settings = jsonDecode(config.readAsStringSync());

    setState(() {
      remoteUrl = settings["url"];
      usernameController.text = settings["username"];
      passwordController.text = settings["password"];
      remember = settings["remember"];
      autoLogin = settings["autoLogin"];
      clientPathController.text = settings["client path"];
      version = settings["version"];
      status = 'app: ${version['app']} resource: ${version['resource']}';
    });

    if (autoLogin) {
      await onLogin();
    }
  }

  void writeConfig() {
    File config = File('$exePath/config.json');

    if (!config.existsSync()) {
      config.createSync();
    }

    remember = autoLogin ? true : remember;

    config.writeAsStringSync(
      jsonEncode({
        "url": "http://1.13.22.82:51530",
        "username": remember ? usernameController.text : '',
        "password": remember ? passwordController.text : '',
        "remember": remember,
        "autoLogin": autoLogin,
        "client path": clientPathController.text,
        "version": version
      }),
    );
  }

  Future<bool> download(String url, String path) async {
    var result = false;

    if (url.isEmpty) return result;

    try {
      var filename = url.substring(url.lastIndexOf('/') + 1, url.length);
      var patchFile = "$exePath/temp/$filename";

      var downloadStatus = '';

      Response resp = await dio.download(url, patchFile,
          onReceiveProgress: (received, total) {
        if (total != -1) {
          downloadStatus = '${(received / total * 100).toStringAsFixed(0)}%';
        }

        setState(() {
          status =
              'app: ${version['app']} resource: ${version['resource']} 正在更新[$downloadStatus]';
        });
      });

      if (resp.statusCode != 200) {
        EasyLoading.showError('获取更新失败');
        return false;
      }

      return await patch(path, filename);
    } on DioError catch (e) {
      if (e.response != null) {
        logger.i(e.response!.data);
        logger.i(e.response!.headers);
        logger.i(e.response!.requestOptions);
      } else {
        logger.i(e.requestOptions);
        logger.i(e.message);
      }

      return result;
    }
  }

  Future<bool> patch(String path, String file) async {
    var result = false;

    var controller = ShellLinesController();
    var shell = Shell(
      throwOnError: false,
      stdout: controller.sink,
      verbose: false,
    );

    controller.stream.listen((event) {
      if (event.characters.string.contains('hpatchz dir patch time:')) {
        result = true;
      }
    });

    await shell.run('patch -f "$path" "$exePath/temp/$file" "$path"');

    return result;
  }

  Future<void> update() async {
    var needUpdate = {'app': true, 'resource': true};

    try {
      setState(() {
        status = '正在检查更新...';
      });

      Response resp = await dio.get("$remoteUrl/api/version/latest",
          options: Options(contentType: "application/json"));

      if (resp.data["status"] == 0) {
        EasyLoading.showError(resp.data["msg"]);
        return;
      }

      setState(() {
        for (var element in resp.data["msg"]) {
          switch (element["name"]) {
            case 'app':
              remoteVersion["app"] = {
                'version': element["version"],
                'force': element["force"],
                'link': element["link"],
              };
              break;
            case 'resource':
              remoteVersion["resource"] = {
                'version': element["version"],
                'force': element["force"],
                'link': element["link"],
              };
              break;
          }
        }
      });

      if (Version.parse(remoteVersion['app']['version']) >
          Version.parse(version['app'])) {
        // download(remoteVersion['app']['link'], exePath).then((value) async {
        //   if (value) {
        //     var patch = File("$exePath/temp/test.patch");
        //     await patch.delete();

        //     setState(() {
        //       version['app'] = remoteVersion["app"]['version'];
        //     });
        //   } else {
        //     status = 'app更新失败';
        //   }
        // });
        setState(() {
          status = '登录器有更新, 请重新下载';
        });
      } else {
        needUpdate['app'] = false;
      }

      if (Version.parse(remoteVersion['resource']['version']) >
          Version.parse(version['resource'])) {
        download(remoteVersion['resource']['link'], clientPathController.text)
            .then((value) async {
          if (value) {
            var patch = File("$exePath/temp/test.patch");
            await patch.delete();

            setState(() {
              version['resource'] = remoteVersion["resource"]['version'];
            });
          } else {
            status = 'resource更新失败';
          }
        });
      } else {
        needUpdate['resource'] = false;
      }

      if (!needUpdate['app']! && !needUpdate['resource']!) {
        setState(() {
          btnDisable = false;
        });
      }

      setState(() {
        status = 'app: ${version['app']} resource: ${version['resource']}';
      });
    } on DioError catch (e) {
      if (e.response != null) {
        logger.i(e.response!.data);
        logger.i(e.response!.headers);
        logger.i(e.response!.requestOptions);
      } else {
        logger.i(e.requestOptions);
        logger.i(e.message);
      }

      setState(() {
        status = '网络连接失败';
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: PreferredSize(
        preferredSize: const Size.fromHeight(kWindowCaptionHeight),
        child: WindowCaption(
          brightness: Brightness.dark,
          title: Row(
            children: [
              Image.asset(
                'images/icon.png',
              ),
              const SizedBox(
                width: 8,
              ),
              const Text(
                'BDO Launcher',
                style: TextStyle(
                  fontFamily: '微软雅黑',
                  color: Colors.white,
                ),
              ),
            ],
          ),
        ),
      ),
      body: Container(
        width: double.infinity,
        height: double.infinity,
        decoration: const BoxDecoration(
          image: DecorationImage(
            fit: BoxFit.fitHeight,
            image: AssetImage(
              'images/bg.jpg',
            ),
          ),
        ),
        child: Column(
          children: [
            Expanded(
              child: Row(
                mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                children: [
                  Expanded(
                    child: Column(
                      children: [
                        Expanded(
                          child: Card(
                            color: const Color.fromARGB(120, 0, 0, 0),
                            child: CarouselSlider(
                              options: CarouselOptions(
                                height: double.infinity,
                                autoPlay: true,
                                enlargeCenterPage: true,
                              ),
                              items: slide.map((item) {
                                return Builder(
                                  builder: (BuildContext context) {
                                    return TextButton(
                                      child: Container(
                                        width: double.infinity,
                                        decoration: BoxDecoration(
                                          image: DecorationImage(
                                            fit: BoxFit.fill,
                                            image: Image.network(
                                              item['img'],
                                              errorBuilder: (context, error,
                                                      stackTrace) =>
                                                  const Icon(Icons.error),
                                            ).image,
                                          ),
                                        ),
                                        child: Column(
                                          children: [
                                            const Spacer(
                                              flex: 6,
                                            ),
                                            Expanded(
                                              child: Container(
                                                width: double.infinity,
                                                decoration: const BoxDecoration(
                                                  color: Color.fromARGB(
                                                      120, 0, 0, 0),
                                                ),
                                                child: Center(
                                                  child: Text(
                                                    '${item['title']}',
                                                    style: const TextStyle(
                                                      fontSize: 24,
                                                      fontFamily: '微软雅黑',
                                                      color: Colors.white,
                                                    ),
                                                  ),
                                                ),
                                              ),
                                            ),
                                          ],
                                        ),
                                      ),
                                      onPressed: () async {
                                        await launchUrl(Uri.parse(remoteUrl));
                                      },
                                    );
                                  },
                                );
                              }).toList(),
                            ),
                          ),
                        ),
                        const SizedBox(
                          height: 8,
                        ),
                        Expanded(
                          child: Card(
                            color: const Color.fromARGB(120, 0, 0, 0),
                            child: ListView.builder(
                              itemCount: news.length,
                              itemBuilder: (BuildContext context, int index) {
                                return SizedBox(
                                  height: 50,
                                  child: TextButton(
                                    child: Row(
                                      children: [
                                        Text(
                                          '${news[index]['title']}',
                                          style: const TextStyle(
                                            color: Colors.white,
                                            fontFamily: '微软雅黑',
                                          ),
                                        ),
                                        const Spacer(),
                                        Text(
                                          news[index]['date']
                                              .toString()
                                              .replaceAll(' +0800', ''),
                                          style: const TextStyle(
                                            color: Colors.white,
                                            fontFamily: '微软雅黑',
                                          ),
                                        ),
                                      ],
                                    ),
                                    onPressed: () async {
                                      await launchUrl(Uri.parse(remoteUrl));
                                    },
                                  ),
                                );
                              },
                            ),
                          ),
                        ),
                      ],
                    ),
                  ),
                  Expanded(
                    child: Image.asset("images/logo.png"),
                  )
                ],
              ),
            ),
            SizedBox(
              height: 142,
              child: Card(
                color: const Color.fromARGB(120, 0, 0, 0),
                child: Padding(
                  padding: const EdgeInsets.all(10),
                  child: Row(
                    children: [
                      Expanded(
                        flex: 6,
                        child: Column(
                            crossAxisAlignment: CrossAxisAlignment.start,
                            children: logined == false
                                ? [
                                    TextField(
                                      controller: usernameController,
                                      style:
                                          const TextStyle(color: Colors.white),
                                      decoration: const InputDecoration(
                                        border: OutlineInputBorder(),
                                        enabledBorder: OutlineInputBorder(
                                          borderSide:
                                              BorderSide(color: Colors.white),
                                        ),
                                        labelText: '用户名',
                                        labelStyle: TextStyle(
                                          color: Colors.white,
                                          fontFamily: '微软雅黑',
                                        ),
                                      ),
                                    ),
                                    const SizedBox(
                                      height: 8,
                                    ),
                                    TextField(
                                      controller: passwordController,
                                      style:
                                          const TextStyle(color: Colors.white),
                                      obscureText: true,
                                      decoration: const InputDecoration(
                                        border: OutlineInputBorder(),
                                        enabledBorder: OutlineInputBorder(
                                          borderSide:
                                              BorderSide(color: Colors.white),
                                        ),
                                        labelText: '密码',
                                        labelStyle: TextStyle(
                                          color: Colors.white,
                                          fontFamily: '微软雅黑',
                                        ),
                                      ),
                                    ),
                                  ]
                                : [
                                    Text(
                                      "用户名: ${usernameController.text}",
                                      style: const TextStyle(
                                        color: Colors.white,
                                        fontSize: 16,
                                        fontFamily: '微软雅黑',
                                      ),
                                    ),
                                    const SizedBox(
                                      height: 8,
                                    ),
                                    Text(
                                      "昵称: ${user['familyname']}",
                                      style: const TextStyle(
                                        color: Colors.white,
                                        fontSize: 16,
                                        fontFamily: '微软雅黑',
                                      ),
                                    ),
                                    const SizedBox(
                                      height: 8,
                                    ),
                                    const Text(
                                      "虚拟币: 0",
                                      style: TextStyle(
                                        color: Colors.white,
                                        fontSize: 16,
                                        fontFamily: '微软雅黑',
                                      ),
                                    ),
                                    const SizedBox(
                                      height: 8,
                                    ),
                                    const Text(
                                      "珍珠: 0",
                                      style: TextStyle(
                                        color: Colors.white,
                                        fontSize: 16,
                                        fontFamily: '微软雅黑',
                                      ),
                                    ),
                                  ]),
                      ),
                      const SizedBox(
                        width: 8,
                      ),
                      SizedBox(
                        width: 100,
                        height: double.infinity,
                        child: Column(
                          mainAxisAlignment: MainAxisAlignment.end,
                          children: [
                            Row(
                              children: [
                                Checkbox(
                                  hoverColor: Colors.orange,
                                  checkColor: Colors.orange,
                                  fillColor:
                                      const MaterialStatePropertyAll<Color>(
                                          Colors.white),
                                  onChanged: (bool? value) {
                                    if (!autoLogin) {
                                      setState(() {
                                        remember = value!;
                                      });
                                    }
                                  },
                                  value: remember,
                                ),
                                const Text(
                                  '记住密码',
                                  style: TextStyle(
                                    color: Colors.white,
                                    fontFamily: '微软雅黑',
                                  ),
                                ),
                              ],
                            ),
                            Row(
                              children: [
                                Checkbox(
                                  hoverColor: Colors.orange,
                                  checkColor: Colors.orange,
                                  fillColor:
                                      const MaterialStatePropertyAll<Color>(
                                          Colors.white),
                                  onChanged: (bool? value) {
                                    setState(() {
                                      autoLogin = value!;

                                      if (autoLogin) {
                                        remember = true;
                                      }
                                    });
                                  },
                                  value: autoLogin,
                                ),
                                const Text(
                                  '自动登录',
                                  style: TextStyle(
                                    color: Colors.white,
                                    fontFamily: '微软雅黑',
                                  ),
                                ),
                              ],
                            ),
                          ],
                        ),
                      ),
                      const SizedBox(
                        width: 8,
                      ),
                      SizedBox(
                        width: 120,
                        height: double.infinity,
                        child: ElevatedButton(
                          style: const ButtonStyle(
                            backgroundColor: MaterialStatePropertyAll<Color>(
                                Color.fromARGB(255, 255, 152, 0)),
                          ),
                          child: const Text(
                            "设置",
                            style: TextStyle(
                              fontSize: 24,
                              fontFamily: '微软雅黑',
                            ),
                          ),
                          onPressed: () {
                            EasyLoading.show(
                              indicator: Container(
                                height: 140,
                                width: 600,
                                decoration: BoxDecoration(
                                  color:
                                      const Color.fromARGB(250, 255, 255, 255),
                                  borderRadius: BorderRadius.circular(5),
                                ),
                                child: Column(
                                  children: [
                                    Expanded(
                                      child: Card(
                                        color: Colors.black,
                                        child: TabBar(
                                          controller: tabController,
                                          tabs: myTabs,
                                        ),
                                      ),
                                    ),
                                    SizedBox(
                                      height: 80,
                                      child: TabBarView(
                                        controller: tabController,
                                        children: myTabs.map((Tab tab) {
                                          switch (myTabs.indexOf(tab)) {
                                            case 0:
                                              return Padding(
                                                padding:
                                                    const EdgeInsets.all(8),
                                                child: Column(
                                                  children: [
                                                    Row(
                                                      children: [
                                                        Expanded(
                                                          flex: 10,
                                                          child: TextField(
                                                            controller:
                                                                clientPathController,
                                                            style:
                                                                const TextStyle(
                                                                    color: Colors
                                                                        .black),
                                                            decoration:
                                                                const InputDecoration(
                                                              border:
                                                                  OutlineInputBorder(),
                                                              enabledBorder:
                                                                  OutlineInputBorder(
                                                                borderSide:
                                                                    BorderSide(
                                                                  color: Colors
                                                                      .black,
                                                                ),
                                                              ),
                                                              labelText:
                                                                  '黑色沙漠客户端位置',
                                                              labelStyle:
                                                                  TextStyle(
                                                                color: Colors
                                                                    .black,
                                                                fontFamily:
                                                                    '微软雅黑',
                                                              ),
                                                            ),
                                                          ),
                                                        ),
                                                        const SizedBox(
                                                          width: 8,
                                                        ),
                                                        SizedBox(
                                                          width: 140,
                                                          height: 60,
                                                          child: ElevatedButton(
                                                            onPressed:
                                                                () async {
                                                              var selected =
                                                                  await FilePicker
                                                                      .platform
                                                                      .getDirectoryPath();

                                                              if (selected !=
                                                                  null) {
                                                                setState(() {
                                                                  clientPathController
                                                                          .text =
                                                                      selected;
                                                                });
                                                              }
                                                            },
                                                            child: const Text(
                                                              '选择',
                                                              style: TextStyle(
                                                                fontSize: 24,
                                                                fontFamily:
                                                                    '微软雅黑',
                                                              ),
                                                            ),
                                                          ),
                                                        ),
                                                      ],
                                                    ),
                                                  ],
                                                ),
                                              );
                                            case 1:
                                              return const Center(
                                                child: Text(
                                                  'This is 1 tab',
                                                  style:
                                                      TextStyle(fontSize: 36),
                                                ),
                                              );
                                            case 2:
                                              return const Center(
                                                child: Text(
                                                  'This is 2 tab',
                                                  style:
                                                      TextStyle(fontSize: 36),
                                                ),
                                              );
                                            default:
                                              return const Center(
                                                child: Text(
                                                  '错误',
                                                  style:
                                                      TextStyle(fontSize: 36),
                                                ),
                                              );
                                          }
                                        }).toList(),
                                      ),
                                    ),
                                  ],
                                ),
                              ),
                            );
                          },
                        ),
                      ),
                      const SizedBox(
                        width: 8,
                      ),
                      SizedBox(
                        width: 120,
                        height: double.infinity,
                        child: ElevatedButton(
                          style: const ButtonStyle(
                            backgroundColor: MaterialStatePropertyAll<Color>(
                                Color.fromARGB(255, 233, 30, 99)),
                          ),
                          child: const Text(
                            "注册",
                            style: TextStyle(
                              fontSize: 24,
                              fontFamily: '微软雅黑',
                            ),
                          ),
                          onPressed: () async {
                            await launchUrl(Uri.parse(remoteUrl));
                          },
                        ),
                      ),
                      const SizedBox(
                        width: 8,
                      ),
                      SizedBox(
                        width: 120,
                        height: double.infinity,
                        child: ElevatedButton(
                          style: btnDisable
                              ? const ButtonStyle(
                                  backgroundColor:
                                      MaterialStatePropertyAll<Color>(
                                          Colors.blueGrey),
                                )
                              : const ButtonStyle(),
                          onPressed: btnDisable
                              ? null
                              : () async {
                                  if (usernameController.value.text.isEmpty ||
                                      passwordController.value.text.isEmpty) {
                                    EasyLoading.showError("用户名或密码不能为空");
                                    return;
                                  }

                                  if (clientPathController.value.text.isEmpty) {
                                    EasyLoading.showError("请先点击设置选择客户端所在的目录");
                                    return;
                                  }

                                  if (logined) {
                                    runGame();
                                  } else {
                                    await onLogin();
                                  }
                                },
                          child: Text(
                            logined ? "启动" : "登录",
                            style: const TextStyle(
                              fontSize: 24,
                              fontFamily: '微软雅黑',
                            ),
                          ),
                        ),
                      ),
                    ],
                  ),
                ),
              ),
            ),
            SizedBox(
              width: double.infinity,
              height: 45,
              child: Card(
                color: const Color.fromARGB(120, 0, 0, 0),
                child: Padding(
                  padding: const EdgeInsets.all(8),
                  child: Text(
                    status,
                    style: const TextStyle(
                      color: Colors.white,
                      fontFamily: '微软雅黑',
                    ),
                  ),
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
