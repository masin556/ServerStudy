from django.urls import path 

from . import views #호출

urlpatterns = [
    path('pybo/', views.index),
]