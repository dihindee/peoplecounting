# peoplecounting
Прототип счетчика людей на видео.

Считает людей, которые прошли через нарисованную пользователем линию (без учета направления пересечения).

Для выбора видео: File>Open video или Ctrl+O.

Линия пересечения рисуется в окне мышью.

Ползунки используются для калибровки фильтров используемых для обнаружения людей.

Режим DebugMode показывает обработанный кадр (imshow странно работает с nanosleep, может зависнуть).

Программа тестировалась на видео, лежащих в папке testVideos.

Сборка и тестирование проходили на Linux.

Скриншот
![Скриншот](https://i.imgur.com/klbFXHM.png)
