# HW3

## B05901082 電機三 楊晟甫

### 1. Environment

- Cygwin on Win10

### 2. Compile

```shell
# type make in command line
$ make
```

### 3. Execute

```shell
# Generate ZhuYin-Big5.map
$ make map
# Run mydisambig for testdata/1.txt~10.txt
$ make run
```

### 4. Work

- Install

  - 用Cygwin裝的話，會遇到FAQ裡面的Q7. 以及FAQ(test)裡面Q3.的問題，需要將 

    $USER_PATH/srilm-1.5.10/lm/src/matherr.c 第 14 ~ 29 行註解掉。

- generate.py
  - 中文字的decode encode有點麻煩，不知道是不是windows的關係，用big5編碼會有沒辦法encode的問題，必須要用big5-hkscs才能順利執行，另外寫檔案的時候也沒辦法先用string串好再寫出，而是得ㄧ個ㄧ個字寫，也是遇到的一個error。

- mydisambig.cpp
  - 先用map把轉換好的 ZhuYin-Big5.map存取起來，分別是一個注音對多個字/一個字對一個字，方便後續處理。
  - 在testing_line的首尾分別加上一個開始以及結束的token，方便在做viterbi的時候運算。
  - 做Viterbi Algorithm，記錄路徑的prob以及其predecessor。
  - 從結束的token backtrack回去，尋找max_prob以及其對應的idx。
  - 輸出修正完的句子。