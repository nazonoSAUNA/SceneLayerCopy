# シーンのレイヤー情報コピペプラグイン

## 動作要件
- Visual C++ 再頒布可能パッケージの2022 x86(32bit)対応の物がインストールされている必要があります【Microsoft Visual C++ 2015-2022 Redistributable(x86)】
- マイクロソフト公式:< https://docs.microsoft.com/ja-jp/cpp/windows/latest-supported-vc-redist >
- AviUtl解説サイト:< https://scrapbox.io/aviutl/Visual_C++_再頒布可能パッケージ >

- 拡張編集0.92のみ動作します

## 仕様
- 現在のシーンのレイヤー情報を[ペースト先]トラックのシーンへコピペします
- AviUtl上部メニューにある設定＞レイヤー情報コピペの設定よりプラグインウィンドウが表示されます
![無題](https://user-images.githubusercontent.com/99536641/204029389-eba1b46d-ea43-4df6-843f-022a89584d1d.png)
- レイヤー名もしくはその他のチェックボックスをクリックすると確認メッセージが出た後、コピペを実施します
- その他とは[レイヤーのロック][レイヤーの表示][座標のリンク][上のオブジェクトでクリッピング]です
