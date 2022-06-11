pathIn = 'test.txt';
pathOut = 'test.resource'

def readAll(file) -> bytes:
    buf = file.readlines()
    data = b''
    for x in buf:
        data += x
    return data

def writeHeader(file, name: str, size: int, comments: tuple = None) -> None:
    file.write(f'"{name}":'.encode())
    if comments:
        for comment in comments:
            if not comment.startswith('_'):
                file.write(f'\n\t"{comment}"'.encode())
    file.write(f'\n\t_resourceSize: {size}\n'.encode())

def main() -> None:
    with open(pathOut, 'wb') as outFile:
        with open(pathIn, 'rb') as inFile:
            inFileData = readAll(inFile)
            writeHeader(outFile, pathIn, len(inFileData))
            outFile.write(inFileData)

if __name__ == '__main__':
    main()
    # input()
