## Libs necessárias
[libgit2](https://github.com/libgit2/libgit2)

## Compilar o Plugin
Compile o plugin com o comando:
```sh
    make
```

## Instalar o Plugin
Copie o arquivo compilado .so e o arquivo .desktop para os diretórios de plugins do Thunar:
```sh
sudo cp myplugin.so /usr/lib/thunarx-3/
sudo cp myplugin.desktop /usr/share/thunarx-3/
```

## Testar o Plugin
Reinicie o Thunar para carregar o novo plugin:
```sh
thunar -q
thunar
```