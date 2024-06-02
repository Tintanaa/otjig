# Разработка алгоритма имитации отжига для задачи job-shop

## Задача цеха (job-shop):
![изображение](https://github.com/Tintanaa/simulated_annealing_demo/assets/99679370/24b624e7-b6ab-469b-9880-f0350ac7a311)

1) https://developers.google.com/optimization/scheduling/job_shop
2) https://en.wikipedia.org/wiki/Job-shop_scheduling
3) https://www.youtube.com/watch?v=UGvc-qujB-o
4) https://towardsdatascience.com/the-job-shop-scheduling-problem-mixed-integer-programming-models-4bbee83d16ab

## Алгоритм имитации отжига (simulated annealing):

<p align="center">
  <img width="1000" height="300" src="https://github.com/Tintanaa/simulated_annealing_demo/assets/99679370/860b1e7f-e47c-4588-9c2c-7b7780bf46f1">
</p>

1) https://ru.wikipedia.org/wiki/%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%B8%D0%BC%D0%B8%D1%82%D0%B0%D1%86%D0%B8%D0%B8_%D0%BE%D1%82%D0%B6%D0%B8%D0%B3%D0%B0
2) https://habr.com/ru/articles/209610/
3) http://www.machinelearning.ru/wiki/index.php?title=%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%B8%D0%BC%D0%B8%D1%82%D0%B0%D1%86%D0%B8%D0%B8_%D0%BE%D1%82%D0%B6%D0%B8%D0%B3%D0%B0
4) https://thecode.media/simulated-annealing/
5) https://www.nayuki.io/page/simulated-annealing-demo

## Вопросы и ответы:
- OC для запуска: любая где можно с c++ работать (сам запускал с Ubuntu 22.04 и Windows 10/11)
- Требовательна ли к железу? Для лучшего результата да, но в целом запустить можно на любом приемлимом компьютере
- Как запустить? Легко!
g++ job.cpp task.cpp graph.cpp jobProcessor.cpp main.cpp -o job-shop

./job-shop <-t/-b> <путь к txt файлу с информацией> [число обрабатываемых работ]

## Дальнейшие планы:
- Переписать код в jupyter notebook

# Приятного использования!
