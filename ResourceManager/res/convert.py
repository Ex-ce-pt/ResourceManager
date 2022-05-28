pathIn = 'tex.png';
pathOut = 'tex1.resource'

def readAll(file) -> bytes:
    buf = file.readlines()
    data = b''
    for x in buf:
        data += x
    return data

def writeHeader(file, name: str, length: int, comments: tuple = None) -> None:
    file.write(f'"{name}":'.encode())
    if not comments == None:
        for x in comments:
            file.write(f'\n\t"{x}"'.encode())
    file.write(f'\n\tlength: {length}\n'.encode())

def main() -> None:
    with open(pathOut, 'wb') as outFile:
        with open(pathIn, 'rb') as inFile:
            inFileData = readAll(inFile)
            writeHeader(outFile, pathIn, len(inFileData))
            outFile.write(inFileData)

if __name__ == '__main__':
    main()
    # input()
