for file in *.cc
do
    BINARY=${file%.cc}
    g++ -std=c++14 -O2 ${file} -lpthread -o ${BINARY}
    ./${BINARY}
    rm -f ${BINARY}
done

