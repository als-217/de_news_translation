from nltk import sent_tokenize
import spacy
import re
options = ('ner', 'textcat', 'senter', 'sentencizer', 'textcat_multilabel', 'entity_ruler', 'entity_linker')
nlp = spacy.load("de_dep_news_trf", disable = options)

def tokenize(paragraph: str) -> list:
        tokenized = []
        sentences = sent_tokenize(paragraph)
        for sent in sentences:
            doc = nlp(re.sub(r'[^\w\s]', '', sent))
            tokenized.append(doc)

        return tokenized
    
PRO_ADVS = ["hier", "dar", "davon", "da", "wo", "dato", "damit", "wohl", "dann", "wovon", "damals",
            "wohin", "woher", "hierher", "hierhin", "dalli", "daher", "darauf", "womöglich", "dazu",
            "wochenlang", "wogegen", "hieran", "woanders", "daheim", "dahinter", "darunter", 
            "darüber", "dagegen", "daran", "dabei", "darum", "dafür", "wol", "hiervon", "daraus",
            "daneben", "woran", "wozwischen", "dadurch", "worin", "dazwischen", "woneben", "wovor",
            "davor", "wohinter", "worüber", "worauf", "worunter", "woraus", "wodurch", "darin", "wonach", "worum", 
            "danach", "wofür", "wozu", "womit", "wobei", "dahin", "hiermit", "das heißt", "dannen", 
            "woll", "hierfür", "woraufhin", "hierbei", "hierdurch", "hierin", "hieraus", "darnach", "hierzu",
            "dafuer", "daraufhin", "hierzulande", "darinnen", "hiernach", "hiernechst", "hierherum", "hiernieden",
            "dabey", "hierein", "hiergegen", "hierauf", "hiernächst", "darinn", "dariñ", "hier und da", "wohlauf",
            "wochentags", "wohlgemerkt", "woandershin", "dann und wann", "wohl eher", "hierwärts", "darob", 
            "dazumal", "darein", "hierselbst", "daselbst", "dawider", "darunten", "woselbst",
            "dahinten", "darinne", "wohlan", "darzu"]

SVPS = ["ab", "an", "auf", "aus", "bei", "da", "durch", "ein", "fern", "fest", "fort", "her", "hin",
        "mit", "nach", "um", "vor", "wieder", "zu", "zurück"]

PREPS = ["ab", "um", "after", "qua", "in", "bei", "dank", "an", "an", "à", "ohne", "mit", "unter", "zwischen", 
         "auf", "ob", "zu", "per", "nach", "aus", "über", "pro", "bar", "inn", "@", "bey", "von", "seit", "hinter", 
         "inner", "neben", "vor", "wider", "bis", "gegen", "gen", "für", "binnen", "durch", "wegen", "gegenüber", 
         "laut", "uff", "entlang", "außer", "oberhalb", "während", "innerhalb", "trotz", "anstatt", "jenseits", 
         "statt", "unterhalb", "diesseits", "außerhalb", "nächst", "kraft", "nahe", "fuer", "mangels", 
         "betreffend", "mang", "gelegentlich", "namens", "ungeachtet", "zuzüglich", "längs", "getreu", 
         "abzüglich", "gemäß", "samt", "aufgrund", "bezüglich", "entsprechend", "angesichts", "voller", "mittels", 
         "vermittels", "vorbei", "ausschließlich", "nebst", "zufolge", "zugunsten", "unweit", "gemäss", "sonder", 
         "inmitten", "anstelle", "sammt", "eingedenk", "mithilfe", "aufseiten", "inklusive", "anhand", "abseits", 
         "einschließlich", "infolge", "hinsichtlich", "zwecks", "anläßlich", "anlässlich", "vermöge", "mitsamt", 
         "betreffs", "võ", "dreiviertel", "auff", "auß", "biß", "ausser", "unbeschadet", "fernab", "weitab", 
         "vong", "behufs", "ausweislich", "mittelst", "einschliesslich", "seitens", "füa", "zeit", "zuungunsten", 
         "auftrags", "zulasten", "innert", "eingerechnet"]

class DeText:
    
    def __init__(self, paragraph: str):
        self.text_ = tokenize(paragraph)
        
    def get_svp(self, word) -> tuple:
        svp = ''
        for child in word.rights:
            if (child.tag_ == "PTKVZ" and child.text in SVPS):
                svp = child
                
        try:
            return (svp.lemma_ + word.lemma_.lower(), word.text.lower() + ' ' + svp.text)
        except AttributeError:
            return (word.lemma_.lower(), word.text.lower())
    
    def get_prf(self, word):
        for child in word.children:
            if child.tag_ == "PRF":
                return (child.lemma_, child.text)
            
        return ('', '')
    
    def get_phrase_prep(self, word) -> tuple:
        prep = ('', '')
        for child in word.children:
            if child.text in PREPS or child.text in PRO_ADVS:
                prep = child
                if prep.text == '':
                    return ('', '')
            
            if child.tag_ == "PROAV":
                to_return = child.text.lower()
                if word[0] == 'w' or word[0] == 'd':
                    if word[2] == 'r':
                        prep = (word[3:], word)
                    else:
                        prep = (word[2:], word)
                else:
                    prep = (word[4:], word)
            elif child.pos_ == "ADP":
                prep = (child.lemma_.lower(), child.text.lower())

        return prep
        
    def get_verb(self, word) -> tuple:
        if (word.tag_ == "VVFIN" or word.tag_ == "VVIMP"):
            verb = self.get_svp(word)
        elif word.tag_ in ["VMFIN", "VMINF", "VAFIN", "VVINF", "VVPP"]:
            verb = (word.lemma_, word.text)

        return tuple([item.lower() for item in verb])
    
    def get_verb_connect(self, word) -> list:
        prf = self.get_prf(word)
        verb = self.get_verb(word)
        prep = self.get_phrase_prep(word)
    
        return [prf, verb, prep]
    
    def get_noun(self, word) -> tuple:
        if word.tag_ in ["NN", "NA"]:
            noun = (word.lemma_, word.text)

        return noun
    
    def get_adj(self, word) -> tuple:
        if word.tag_ == "ADJA":
            adj = (word.lemma_, word.text)
            
        return adj
    
    def get_adv(self, word) -> tuple:
        if word.tag_ in ["ADJD", "ADV", "PROAV"]:
            adj = (word.lemma_, word.text)
            
        return adj
    
    def get_prep(self, word) -> tuple:
        if word.tag_ in ["APPRART", "APPR", "APPO", "APZR"]:
            prep = (word.lemma_, word.text)
            
        return prep
    
    def get_word(self, sent_idx: int, word_idx: list, connect: bool) -> tuple:
        to_get = self.text_[sent_idx][word_idx]
        if to_get.pos_ in ["VERB", "AUX"]:
            if connect == False:
                word = (self.get_verb(to_get), "VERB")
            else:
                word = (self.get_verb_connect(to_get), to_get.pos_)
        elif to_get.pos_ == "NOUN":
            word = (self.get_noun(to_get), "NOUN")
        elif to_get.pos_ == "ADP":
            word = (self.get_prep(to_get), "ADP")
        elif to_get.pos_ == "ADV":
            word = (self.adv(to_get), "ADV")
        elif to_get.pos_ == "ADV":
            word = (self.adj(to_get), "ADJ")

        return word

