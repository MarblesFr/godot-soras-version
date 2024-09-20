rm -rf Godot.app/Contents/MacOS/Godot
cp bin/godot.macos.editor.arm64 Godot.app/Contents/MacOS/Godot
chmod +x Godot.app/Contents/MacOS/Godot
codesign --force --timestamp --options=runtime --entitlements misc/dist/macos/editor.entitlements -s - Godot.app