# C-Extension
C-based extension for serializing/deserializing data to Protocol Buffers (protobuf)

## Structure
- `appsinstalled.proto` — Protocol buffers schema
- `c_protobuf.c` — C implementation -> save/load func
- `setup_protobuf.py` — C extension building script
- `c_protobuf_test.py` — Example usage script
- `setup_testplugin.py`, `test_plugin.py` — Extra plugin example (func add()) for learning

## Install
Install `protobuf-c`:  
```bash
brew install protobuf-c
```

Generate Protobuf C Files:  
```bash
protoc --c_out=. appsinstalled.proto
```

## Build the extension
```shell
python3 setup_protobuf.py build_ext --inplace
```

## Example usage
```shell
python3 c_protobuf_test.py
```
Input example:  
```
idfa e7e1a50c0ec2747ca56cd9e1558c0d7c 67.7835424444 -22.8044005471 7942,8519,4232,3
```

Output example:  
```
Writing to file
'idfa', '123abc', 55.75, 37.61, [1, 2, 3, 4, 5]
Writing done
------------------------------
Load written file
Data: ('idfa', '123abc', 55.75, 37.61, [1, 2, 3, 4, 5])
```

## Useful Link
[protobuf-c Examples](https://github.com/protobuf-c/protobuf-c/wiki/Examples)