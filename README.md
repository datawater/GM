
# GM

GM - Grandmaster machine. A virtual machine with its own assembler (coming soon: disassembler)!

- [Running Locally](#Run-Locally)
- [TODO](#TODO)
- [Screenshots](#Screenshots)
- [Authors](#Authors)
<p align="center" width="100%">
    <img src="https://i.postimg.cc/1tfjn1KF/logo-horizontal.png">
    <br>
    <img src="https://github.com/datawater/gm/actions/workflows/ci.yml/badge.svg?style=flat-square">
    <img src="https://img.shields.io/badge/License-GPL%20v3-blue.svg?style=flat-square>
</p>

## Run-Locally

Clone the project

```shell
$ git clone https://github.com/datawater/GM
```

Go to the project directory

```shell
$ cd GM
```

Build the project

```shell
# For the release build
$ make release && make test-release

# For the development build (not optimized, with debug info, but a faster build)
$ make && make test
```


## TODO

```shell
$ make test
```

- [ ]  ./src/include/gasm.h:6: // TODO: implement new instructions such as: if (cmp)
- [ ]  A disassembler
## Screenshots

[![App Screenshot](https://i.postimg.cc/7Pmtw6ML/Screenshot-from-2022-08-01-15-11-25.png)](https://postimg.cc/QVKJqjzL)
## License

See the license in the "LICENSE" File

[LICENSE](/LICENSE)
## Feedback

If you have any feedback, please reach out to me at 
[datawater1@gmail.com](mailto:datawater1@gmail.com&subject=Feedback%20For%20GM) 
<br>
Or open an issue!

## TODO

```shell
$ make test
```

- [ ]  ./src/include/gasm.h:6: // TODO: implement new instructions such as: if (cmp)
- [ ]  A disassembler
## Authors

- [@datawater](https://www.github.com/datawater)