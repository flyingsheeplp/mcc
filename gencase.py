def tokenlized(s):
	return s.split();

def gencase(s):
	L = [];
	for sym in s:
		L.append("case '"+sym+"':");
	return L;

def catcase(c_list):
	str="";
	for c in c_list:
		str = str + c;
	return str;

if __name__ == "__main__":
    print ('Gen case for scanner');
    print(catcase(gencase(tokenlized("a b c d e f g h i j k l m n o p q  r s t u v w x y z A B C D E F G H I J K L M N O P Q R S T U V W X Y Z _"))));

    print(catcase(gencase(tokenlized("0 1 2 3 4 5 6 7 8 9"))));