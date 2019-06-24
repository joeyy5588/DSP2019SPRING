# DSP 2019SPRING  Final Project

## B05901011 許秉倫 B05901082 楊晟甫

## How to execute

#### 1. Web Crawler
```bash
$ cd scraper/
$ python3 scrape.py
```

#### 2. Search part

```bash
# 在兩個terminal中分別輸入以下指令(先開啟bert server再執行搜尋)
# Please download bert pretrained model from here and put it under the folder:
# https://storage.googleapis.com/bert_models/2018_11_03/chinese_L-12_H-768_A-12.zip
$ bert-serving-start -model_dir chinese_L-12_H-768_A-12/
$ python3 search.py
```

#### 3. Parameters
```python
alpha = 1, beta = 0.75, gamma = 0.15
Top-K = 10, iteration number = 10
```

#### 4. File structure 
```
.
├── embedding
│   └── embedding_bert.pkl (course name <-> embedding vector dict)
├── course.pkl (course number <-> course name dict)
├── transform.py
├── server.py
├── scraper
│   ├── parse.py
│   ├── scrape.py
│   ├── test.py
│   └── vis.py
└── search.py
```