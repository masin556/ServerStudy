from django.shortcuts import render

# Create your views here.
from django.http import HttpResponse #간단한 응답 <테스트용>

#urls.py 에 -> views.py index. 함수를 만들어야 한다. 
#리퀘스트 매개변수를 넣어줘야한다.
#요청을 받았기에 본인이 받아들이겠다는 request
def index(request):
    return HttpResponse("안녕하시오. pybo에 온 것을 환영하오.") # 전달