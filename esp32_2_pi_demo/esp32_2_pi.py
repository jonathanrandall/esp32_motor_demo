import requests
URL_esp32 = "http://192.168.1.211"
mot1 = 300
mot2 = 3
str1=URL_esp32 + f"/control?var=o&val={int(mot1)}_{int(mot2)}"





if __name__ == "__main__":
    print(str1)
    tmp1=requests.get(str1)
    numbers = list(map(int, ((tmp1.content).decode('utf-8')).split()))
    print(numbers)
    tmp1=requests.get(str1)
    numbers = list(map(int, ((tmp1.content).decode('utf-8')).split()))
    print(numbers)
    
    