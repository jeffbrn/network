# Network flatbuffers example

## Building

```bash
$ cmake -B build -S .
```

## Compiling the IDL
If flatbuffers is locally installed:
```shell
flatc -o include/messages --cpp --scoped-enums idl/test.fbs
```
Otherwise, you can call the executable:
```shell
build/_deps/flatbuffers-build/flatc
```
with the above arguments

## External Dependencies

### Flatbuffers
Documentation can be found [here](https://flatbuffers.dev/flatbuffers_guide_use_cpp.html)

Source code can be found [here](https://github.com/google/flatbuffers/tree/master/include/flatbuffers)
