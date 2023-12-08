from django import forms  #장고에 있는 forms라는 테이블 불러오기
from pybo.models import Question, Answer #질문을 불러오는 것이기에 Question 테이블을 불러와준다. 

class QuestionForm(forms.ModelForm): #modelForm : 입력할 수 있도록 form을 제공해준다.
    class Meta: #overriding 
        #내가 무언가 작성할 수 있는 형태로 만들어 준다.
        model = Question
        #직접 입력을 위한
        fields = ['subject', 'content']
        #수동폼을 위해
        # widgets = {
        #     'subject': forms.TextInput(attrs={'class': 'form-control'}),
        #     'content': forms.Textarea(attrs={'class': 'form-control', 'rows': 10, 'placeholder': "질문 상세내용 입려하세요"}),
        # }
                
        labels = {
            'subject': '제목',
            'content': '내용',
        }  
        
        
class AnswerForm(forms.ModelForm):
    class Meta:
        model = Answer
        fields = ['content']
        labels = {
            'content' : '답변내용',
        }