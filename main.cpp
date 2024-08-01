#include <iostream>  // 標準入出力ストリームを使用するためのヘッダー
#include <fstream>   // ファイル入出力を扱うためのヘッダー
#include <vector>    // ベクターコンテナを使用するためのヘッダー
#include <memory>    // スマートポインタを使用するためのヘッダー
#include <string>    // 文字列操作を行うためのヘッダー
#include <sstream>   // 文字列ストリームを使用するためのヘッダー

using namespace std;

// Shapeクラス
class Shape {
public:
    virtual ~Shape() = default; // 仮想デストラクタ
    virtual string toOpenSCAD() const = 0; // OpenSCAD形式の文字列を返す純粋仮想関数
    virtual void setPosition(double x, double y, double z) = 0; // 位置を設定する純粋仮想関数
    virtual string getPosition() const = 0; // 位置を取得する純粋仮想関数
};

// Cubeクラス
class Cube : public Shape {
    double width, height, depth; // 立方体の幅、高さ、奥行き
    double posX, posY, posZ; // 立方体の位置（x, y, z）
public:
    Cube(double w, double h, double d) : width(w), height(h), depth(d), posX(0), posY(0), posZ(0) {}
    void setPosition(double x, double y, double z) override {
        posX = x; posY = y; posZ = z; // 位置を設定
    }
    string getPosition() const override {
        // 位置を表すOpenSCADコードを返す
        return "translate([" + to_string(posX) + ", " + to_string(posY) + ", " + to_string(posZ) + "]) ";
    }
    string toOpenSCAD() const override {
        // 立方体のOpenSCADコードを返す
        return getPosition() + "cube([" + to_string(width) + ", " + to_string(height) + ", " + to_string(depth) + "]);";
    }
};

// Sphereクラス
class Sphere : public Shape {
    double radius; // 球の半径
    double posX, posY, posZ; // 球の位置（x, y, z）
public:
    Sphere(double r) : radius(r), posX(0), posY(0), posZ(0) {}
    void setPosition(double x, double y, double z) override {
        posX = x; posY = y; posZ = z; // 位置を設定
    }
    string getPosition() const override {
        // 位置を表すOpenSCADコードを返す
        return "translate([" + to_string(posX) + ", " + to_string(posY) + ", " + to_string(posZ) + "]) ";
    }
    string toOpenSCAD() const override {
        // 球のOpenSCADコードを返す
        return getPosition() + "sphere(r=" + to_string(radius) + ");";
    }
};

// Cylinderクラス
class Cylinder : public Shape {
    double radius, height; // 円柱の半径と高さ
    double posX, posY, posZ; // 円柱の位置（x, y, z）
public:
    Cylinder(double r, double h) : radius(r), height(h), posX(0), posY(0), posZ(0) {}
    void setPosition(double x, double y, double z) override {
        posX = x; posY = y; posZ = z; // 位置を設定
    }
    string getPosition() const override {
        // 位置を表すOpenSCADコードを返す
        return "translate([" + to_string(posX) + ", " + to_string(posY) + ", " + to_string(posZ) + "]) ";
    }
    string toOpenSCAD() const override {
        // 円柱のOpenSCADコードを返す
        return getPosition() + "cylinder(h=" + to_string(height) + ", r=" + to_string(radius) + ");";
    }
};

// Frustumクラス
class Frustum : public Shape {
    double bottomRadius, topRadius, height; // 円錐台の底面半径、上面半径、高さ
    double posX, posY, posZ; // 円錐台の位置（x, y, z）
public:
    Frustum(double br, double tr, double h) : bottomRadius(br), topRadius(tr), height(h), posX(0), posY(0), posZ(0) {}
    void setPosition(double x, double y, double z) override {
        posX = x; posY = y; posZ = z; // 位置を設定
    }
    string getPosition() const override {
        // 位置を表すOpenSCADコードを返す
        return "translate([" + to_string(posX) + ", " + to_string(posY) + ", " + to_string(posZ) + "]) ";
    }
    string toOpenSCAD() const override {
        // 円錐台のOpenSCADコードを返す
        return getPosition() + "cylinder(h=" + to_string(height) + ", r1=" + to_string(bottomRadius) + ", r2=" + to_string(topRadius) + ");";
    }
};

// Torusクラス
class Torus : public Shape {
    double majorRadius, minorRadius; // トーラスの大半径と小半径
    double posX, posY, posZ; // トーラスの位置（x, y, z）
public:
    Torus(double mr, double nr) : majorRadius(mr), minorRadius(nr), posX(0), posY(0), posZ(0) {}
    void setPosition(double x, double y, double z) override {
        posX = x; posY = y; posZ = z; // 位置を設定
    }
    string getPosition() const override {
        // 位置を表すOpenSCADコードを返す
        return "translate([" + to_string(posX) + ", " + to_string(posY) + ", " + to_string(posZ) + "]) ";
    }
    string toOpenSCAD() const override {
        // トーラスのOpenSCADコードを返す
        return getPosition() + "rotate_extrude(angle=360) translate([" + to_string(majorRadius) + ", 0, 0]) circle(r=" + to_string(minorRadius) + ");";
    }
};

// 独自の関数：形状入力処理
void inputShape(vector<shared_ptr<Shape>>& shapes) {
    int choice;
    while (true) {
        cout << "追加する形状を選択してください (1: 立方体, 2: 球, 3: 円柱, 4: 円錐台, 5: トーラス, 0: 終了): ";
        cin >> choice; // ユーザーに形状の選択を求める
        if (choice == 0) break; // 0が入力されたらループ終了

        if (choice == 1) {
            double width, height, depth, x, y, z;
            cout << "立方体の幅、高さ、奥行きを入力してください: ";
            cin >> width >> height >> depth; // 立方体の寸法を入力
            cout << "立方体のx, y, z位置を入力してください: ";
            cin >> x >> y >> z; // 立方体の位置を入力
            auto cube = make_shared<Cube>(width, height, depth); // 立方体のオブジェクトを作成
            cube->setPosition(x, y, z); // 位置を設定
            shapes.push_back(cube); // ベクターに追加
        }
        else if (choice == 2) {
            double radius, x, y, z;
            cout << "球の半径を入力してください: ";
            cin >> radius; // 球の半径を入力
            cout << "球のx, y, z位置を入力してください: ";
            cin >> x >> y >> z; // 球の位置を入力
            auto sphere = make_shared<Sphere>(radius); // 球のオブジェクトを作成
            sphere->setPosition(x, y, z); // 位置を設定
            shapes.push_back(sphere); // ベクターに追加
        }
        else if (choice == 3) {
            double radius, height, x, y, z;
            cout << "円柱の半径と高さを入力してください: ";
            cin >> radius >> height; // 円柱の半径と高さを入力
            cout << "円柱のx, y, z位置を入力してください: ";
            cin >> x >> y >> z; // 円柱の位置を入力
            auto cylinder = make_shared<Cylinder>(radius, height); // 円柱のオブジェクトを作成
            cylinder->setPosition(x, y, z); // 位置を設定
            shapes.push_back(cylinder); // ベクターに追加
        }
        else if (choice == 4) {
            double bottomRadius, topRadius, height, x, y, z;
            cout << "円錐台の底面半径、上面半径、高さを入力してください: ";
            cin >> bottomRadius >> topRadius >> height; // 円錐台の寸法を入力
            cout << "円錐台のx, y, z位置を入力してください: ";
            cin >> x >> y >> z; // 円錐台の位置を入力
            auto frustum = make_shared<Frustum>(bottomRadius, topRadius, height); // 円錐台のオブジェクトを作成
            frustum->setPosition(x, y, z); // 位置を設定
            shapes.push_back(frustum); // ベクターに追加
        }
        else if (choice == 5) {
            double majorRadius, minorRadius, x, y, z;
            cout << "トーラスの大半径と小半径を入力してください: ";
            cin >> majorRadius >> minorRadius; // トーラスの大半径と小半径を入力
            cout << "トーラスのx, y, z位置を入力してください: ";
            cin >> x >> y >> z; // トーラスの位置を入力
            auto torus = make_shared<Torus>(majorRadius, minorRadius); // トーラスのオブジェクトを作成
            torus->setPosition(x, y, z); // 位置を設定
            shapes.push_back(torus); // ベクターに追加
        }
        else {
            cout << "無効な選択です。もう一度試してください。" << endl;
        }
    }
}

// 独自の関数：既存ファイル読み込み処理
string readExistingFile(const string& filepath) {
    ifstream file(filepath); // 指定されたパスのファイルを開く
    if (!file) {
        cerr << "既存のファイルを開けませんでした。新しいファイルとして処理します。" << endl;
        return ""; // ファイルがない場合は空文字列を返す
    }

    stringstream buffer; // 文字列ストリームを作成
    buffer << file.rdbuf(); // ファイルの内容をバッファに読み込む
    return buffer.str(); // バッファの内容を文字列として返す
}

// 独自の関数：ファイル書き込み処理
void writeFile(const vector<shared_ptr<Shape>>& shapes, const string& filepath, bool append) {
    string existingContent = append ? readExistingFile(filepath) : ""; // 既存ファイルの内容を取得

    ofstream file(filepath); // ファイルを書き込みモードで開く
    if (!file) {
        cerr << "ファイルを開けませんでした。" << endl;
        exit(1); // エラーで終了
    }

    file << existingContent; // 既存の内容をファイルに書き込む

    // begin()とend()を使ったイテレータの使用
    for (auto it = shapes.begin(); it != shapes.end(); ++it) {
        file << (*it)->toOpenSCAD() << endl; // 新しい形状をファイルに書き込む
    }
    file.close(); // ファイルを閉じる
}

int main() {
    vector<shared_ptr<Shape>> shapes; // 形状のベクター
    inputShape(shapes); // 形状の入力を受け付ける

    string directory, filename, filepath; // ディレクトリ、ファイル名、ファイルパス
    char choice, appendChoice;
    bool append = false;

    cout << "保存場所を指定しますか？ (y/n): ";
    cin >> choice; // 保存場所を指定するかどうかの確認

    if (choice == 'y' || choice == 'Y') {
        cout << "保存するディレクトリを入力してください (例: ./, /home/user/, C:\\Users\\user\\): ";
        cin >> directory; // 保存するディレクトリを入力
    }
    else {
        directory = "./"; // デフォルトのディレクトリ
        cout << "保存場所を指定しませんでしたので、デフォルトの場所に保存します。" << endl;
    }

    cout << "保存するファイル名を入力してください（拡張子は不要）: ";
    cin >> filename; // ファイル名を入力

    filepath = directory + filename + ".scad"; // ファイルパスを生成

    // 既存ファイルに内容を追加するかどうかの確認
    cout << "既存のファイルがあれば内容を追加しますか？ (y/n): ";
    cin >> appendChoice;
    if (appendChoice == 'y' || appendChoice == 'Y') {
        append = true;
    }

    writeFile(shapes, filepath, append); // ファイルに書き込み

    cout << "ファイルが保存されました: " << filepath << endl; // ファイル保存完了メッセージ

    return 0; // プログラムの終了
}
