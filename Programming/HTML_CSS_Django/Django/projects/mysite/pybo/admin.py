from django.contrib import admin
from .models import Question, Answer


#검색기능 상속 오버라이딩 해줘야한다.
class QuestionAdmin(admin.ModelAdmin):
    search_fields = ['subject']
    
# Register your models here.
admin.site.register(Question, QuestionAdmin) #Question 등록
admin.site.register(Answer) #Answer 등록