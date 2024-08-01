#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <sstream>

using namespace std;

// 抽象クラス Shape: 基本的な形状を表すクラス。各形状クラスはこれを継承して具象化する。
class Shape {
public:
    virtual ~Shape() = default; // デストラクタ
    virtual string toOpenSCAD() const = 0; // OpenSCAD形式のコードを生成する純粋仮想関数。
    virtual void setPosition(double x, double y, double z) = 0; // 位置を設定する純粋仮想関数。
    virtual string getPosition() const = 0; // 位置をOpenSCAD形式で取得する純粋仮想関数。
};

// 立方体を表すクラス Cube
class Cube : public Shape {
    double width, height, depth; // 立方体の幅、高さ、奥行き。
    double posX, posY, posZ; // 立方体の位置。
public:
    // コンストラクタで幅、高さ、奥行きを初期化。位置は(0,0,0)。
    Cube(double w, double h, double d) : width(w), height(h), depth(d), posX(0), posY(0), posZ(0) {}

    // 位置を設定する関数
    void setPosition(double x, double y, double z) override {
        posX = x;
        posY = y;
        posZ = z;
    }

    // 位置をOpenSCAD形式で返す関数
    string getPosition() const override {
        return "translate([" + to_string(posX) + ", " + to_string(posY) + ", " + to_string(posZ) + "]) ";
    }

    // OpenSCAD形式のコードを生成する関数
    string toOpenSCAD() const override {
        return getPosition() + "cube([" + to_string(width) + ", " + to_string(height) + ", " + to_string(depth) + "]);";
    }
};

// 球を表すクラス Sphere
class Sphere : public Shape {
    double radius; // 球の半径
    double posX, posY, posZ; // 球の位置
public:
    // コンストラクタで半径を初期化。位置は(0,0,0)。
    Sphere(double r) : radius(r), posX(0), posY(0), posZ(0) {}

    // 位置を設定する関数
    void setPosition(double x, double y, double z) override {
        posX = x;
        posY = y;
        posZ = z;
    }

    // 位置をOpenSCAD形式で返す関数
    string getPosition() const override {
        return "translate([" + to_string(posX) + ", " + to_string(posY) + ", " + to_string(posZ) + "]) ";
    }

    // OpenSCAD形式のコードを生成する関数
    string toOpenSCAD() const override {
        return getPosition() + "sphere(r=" + to_string(radius) + ");";
    }
};

// 円柱を表すクラス Cylinder
class Cylinder : public Shape {
    double radius, height; // 円柱の半径と高さ
    double posX, posY, posZ; // 円柱の位置
public:
    // コンストラクタで半径と高さを初期化。位置は(0,0,0)。
    Cylinder(double r, double h) : radius(r), height(h), posX(0), posY(0), posZ(0) {}

    // 位置を設定する関数
    void setPosition(double x, double y, double z) override {
        posX = x;
        posY = y;
        posZ = z;
    }

    // 位置をOpenSCAD形式で返す関数
    string getPosition() const override {
        return "translate([" + to_string(posX) + ", " + to_string(posY) + ", " + to_string(posZ) + "]) ";
    }

    // OpenSCAD形式のコードを生成する関数
    string toOpenSCAD() const override {
        return getPosition() + "cylinder(h=" + to_string(height) + ", r=" + to_string(radius) + ");";
    }
};

// 円錐台を表すクラス Frustum
class Frustum : public Shape {
    double bottomRadius, topRadius, height; // 円錐台の底面半径、上面半径、高さ
    double posX, posY, posZ; // 円錐台の位置
public:
    // コンストラクタで各パラメータを初期化。位置は(0,0,0)。
    Frustum(double br, double tr, double h) : bottomRadius(br), topRadius(tr), height(h), posX(0), posY(0), posZ(0) {}

    // 位置を設定する関数
    void setPosition(double x, double y, double z) override {
        posX = x;
        posY = y;
        posZ = z;
    }

    // 位置をOpenSCAD形式で返す関数
    string getPosition() const override {
        return "translate([" + to_string(posX) + ", " + to_string(posY) + ", " + to_string(posZ) + "]) ";
    }

    // OpenSCAD形式のコードを生成する関数
    string toOpenSCAD() const override {
        return getPosition() + "cylinder(h=" + to_string(height) + ", r1=" + to_string(bottomRadius) + ", r2=" + to_string(topRadius) + ");";
    }
};

// トーラスを表すクラス Torus
class Torus : public Shape {
    double majorRadius, minorRadius; // トーラスの輪の半径と断面の半径
    double posX, posY, posZ; // トーラスの位置
public:
    // コンストラクタで各パラメータを初期化。位置は(0,0,0)。
    Torus(double mr, double nr) : majorRadius(mr), minorRadius(nr), posX(0), posY(0), posZ(0) {}

    // 位置を設定する関数
    void setPosition(double x, double y, double z) override {
        posX = x;
        posY = y;
        posZ = z;
    }

    // 位置をOpenSCAD形式で返す関数
    string getPosition() const override {
        return "translate([" + to_string(posX) + ", " + to_string(posY) + ", " + to_string(posZ) + "]) ";
    }

    // OpenSCAD形式のコードを生成する関数
    string toOpenSCAD() const override {
        return getPosition() + "rotate_extrude(angle=360) translate([" + to_string(majorRadius) + ", 0, 0]) circle(r=" + to_string(minorRadius) + ");";
    }
};

// 形状の入力処理を行う関数
void inputShape(vector<shared_ptr<Shape>>& shapes) {
    int choice;
    while (true) {
        // ユーザーに追加する形状を選択させる
        cout << "追加する形状を選択してください (1: 立方体, 2: 球, 3: 円柱, 4: 円錐台, 5: トーラス, 0: 終了): ";
        cin >> choice;
        if (choice == 0) break;

        if (choice == 1) {
            double width, height, depth, x, y, z;
            // 立方体の各寸法を入力
            cout << "立方体の幅、高さ、奥行きを入力してください: ";
            cin >> width >> height >> depth;
            // 立方体の位置を入力
            cout << "立方体のx, y, z位置を入力してください: ";
            cin >> x >> y >> z;
            // 立方体を生成してリストに追加
            auto cube = make_shared<Cube>(width, height, depth);
            cube->setPosition(x, y, z);
            shapes.push_back(cube);
        }
        else if (choice == 2) {
            double radius, x, y, z;
            // 球の半径を入力
            cout << "球の半径を入力してください: ";
            cin >> radius;
            // 球の位置を入力
            cout << "球のx, y, z位置を入力してください: ";
            cin >> x >> y >> z;
            // 球を生成してリストに追加
            auto sphere = make_shared<Sphere>(radius);
            sphere->setPosition(x, y, z);
            shapes.push_back(sphere);
        }
        else if (choice == 3) {
            double radius, height, x, y, z;
            // 円柱の半径と高さを入力
            cout << "円柱の半径と高さを入力してください: ";
            cin >> radius >> height;
            // 円柱の位置を入力
            cout << "円柱のx, y, z位置を入力してください: ";
            cin >> x >> y >> z;
            // 円柱を生成してリストに追加
            auto cylinder = make_shared<Cylinder>(radius, height);
            cylinder->setPosition(x, y, z);
            shapes.push_back(cylinder);
        }
        else if (choice == 4) {
            double bottomRadius, topRadius, height, x, y, z;
            // 円錐台の底面半径、上面半径、高さを入力
            cout << "円錐台の底面半径、上面半径、高さを入力してください: ";
            cin >> bottomRadius >> topRadius >> height;
            // 円錐台の位置を入力
            cout << "円錐台のx, y, z位置を入力してください: ";
            cin >> x >> y >> z;
            // 円錐台を生成してリストに追加
            auto frustum = make_shared<Frustum>(bottomRadius, topRadius, height);
            frustum->setPosition(x, y, z);
            shapes.push_back(frustum);
        }
        else if (choice == 5) {
            double majorRadius, minorRadius, x, y, z;
            // トーラスの輪の半径と断面の半径を入力
            cout << "トーラスの輪の半径と断面の半径を入力してください: ";
            cin >> majorRadius >> minorRadius;
            // トーラスの位置を入力
            cout << "トーラスのx, y, z位置を入力してください: ";
            cin >> x >> y >> z;
            // トーラスを生成してリストに追加
            auto torus = make_shared<Torus>(majorRadius, minorRadius);
            torus->setPosition(x, y, z);
            shapes.push_back(torus);
        }
        else {
            // 無効な選択肢の処理
            cout << "無効な選択です。もう一度試してください。" << endl;
        }
    }
}

// 既存ファイルを読み込み内容を文字列ストリームに保存する関数
void readExistingFile(stringstream& existingContent, const string& filepath) {
    ifstream file(filepath);
    if (!file) {
        cerr << "既存のファイルを開けませんでした。新しいファイルを作成します。" << endl;
    }
    else {
        existingContent << file.rdbuf(); // ファイルの内容を文字列ストリームに読み込む
        file.close(); // ファイルを閉じる
    }
}

// 新規ファイルまたは既存ファイルに形状データを書き込む関数
void writeFile(const vector<shared_ptr<Shape>>& shapes, const string& filepath, bool append) {
    stringstream existingContent;

    // 追加モードが選択された場合のみ既存ファイルの内容を読み込む
    if (append) {
        readExistingFile(existingContent, filepath);
    }

    ofstream file(filepath); // ファイル出力ストリームを開く
    if (!file) {
        cerr << "ファイルを開けませんでした。" << endl;
        exit(1);
    }

    // 既存の内容を先に書き込み
    file << existingContent.str();

    // 新しい形状を追加
    for (const auto& shape : shapes) {
        file << shape->toOpenSCAD() << endl;
    }

    file.close(); // ファイルを閉じる
}

int main() {
    vector<shared_ptr<Shape>> shapes; // 形状オブジェクトのポインタを格納するベクター
    inputShape(shapes); // ユーザーから形状情報を入力させる

    string directory, filename, filepath; // ディレクトリ、ファイル名、ファイルパス
    char choice;
    bool append = false;

    // 既存ファイルに追加するかどうかを尋ねる
    cout << "既存のファイルに追加しますか？ (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        append = true;
    }

    // 保存場所を指定するかどうかを尋ねる
    cout << "保存場所を指定しますか？ (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        // ユーザーに保存するディレクトリを入力させる
        cout << "保存するディレクトリを入力してください (例: ./, /home/user/, C:\\Users\\user\\): ";
        cin >> directory;
    }
    else {
        // デフォルトのディレクトリを設定
        directory = "./";
        cout << "保存場所を指定しませんでしたので、デフォルトの場所に保存します。" << endl;
    }

    // 保存するファイル名を入力させる
    cout << "保存するファイル名を入力してください（拡張子は不要）: ";
    cin >> filename;

    // ファイルパスを生成
    filepath = directory + filename + ".scad";
    // 形状データを書き込む
    writeFile(shapes, filepath, append);

    // 保存完了メッセージを表示
    cout << "ファイルが保存されました: " << filepath << endl;

    return 0; // プログラム終了
}
