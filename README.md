### Download the latest version from the releases && extract it
```
tar xzf libcrypto-*.tar.gz
cd libcrypto-*
sudo ./install.sh
```

## Manual download
```
curl -L https://github.com/ridwanalmahmud/libcrypto/releases/latest/download/libcrypto.tar.gz
tar xzf libcrypto.tar.gz
cd libcrypto
sudo ./install.sh
```

## Make from the source
#### Prerequisites
```
pacman -S clang make
```
```
git clone https://github.com/ridwanalmahmud/libcrypto.git
cd libcrypto
make clean
make
sudo make install
```
