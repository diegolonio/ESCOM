import re

import nltk
import pandas as pd
import spacy

# --- Descarga de recursos -------------------------------------------------
try:
    nltk.data.find("corpora/treebank")
except LookupError:
    nltk.download("book")

try:
    nltk.data.find("taggers/averaged_perceptron_tagger_eng")
except LookupError:
    nltk.download("averaged_perceptron_tagger_eng")

# --- Corpus: Wall Street Journal (text7) ----------------------------------
from nltk import book
from nltk.book import text7

text = text7
books = book
print(books)

print(text)
print(type(text))

print(" ".join(word for word in text[:10]))

print(len(text))
print(text[:10])

print(text[100650:])

fdist = nltk.FreqDist(text)
print(fdist)

# --- POS tagging ----------------------------------------------------------
nlp = spacy.load("en_core_web_sm")

sentence = "When I arrived, the dog in the yard barked."
words = re.findall(r'\w+|[^\s\w]+', sentence)
print(words)

# NLTK: solo etiquetas Penn Treebank -> 2 filas
pos_tags = nltk.pos_tag([token for token in words if token not in [",", "."]])
print(pd.DataFrame(pos_tags).T)

# spaCy: Penn (tag_) + Universal Dependencies (pos_) -> 3 filas
# `word` es un Token, no una cadena; cambia a `word.text` si necesitas str
spacy_pos_tagged = [(word, word.tag_, word.pos_) for word in nlp(sentence)]
print(f"\n{pd.DataFrame(spacy_pos_tagged).T}")
