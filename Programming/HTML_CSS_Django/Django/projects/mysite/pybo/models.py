from django.db import models

# Create your models here.
#만들고자 하는 데이터베이스의 table이름이 들어오면 된다. 
class Question(models.Model): #상속에 대한 개념으로 기능을 끌어와 줘야 한다. 
    #상속했으니 부모클래스의 있는 것을 사용가능하다.
    subject = models.CharField(max_length=200) #질문 을 200길이로 제한 객체로 subject를 만들어주었다.
    #질문에 대한 부가설명 상세 
    content = models.TextField() #textfield는 글자수 제한을 두지 않음. 
    #질문이 등록된 시간
    create_date = models.DateTimeField() #날짜와 시간에 대한 기록


    def __str__(self):
        return self.subject #Question은 문자열로 찾으면 subject를 보여준다. 

#답변 테이블
class Answer(models.Model):
    #위 것과 함께 연동을 시켜줍니다.
    question = models.ForeignKey(Question, on_delete=models.CASCADE) #외래키 가져오겠다는 지워지면 같이 지워줌
    #답변에 대한 내용
    content = models.TextField()
    create_date = models.DateTimeField()
