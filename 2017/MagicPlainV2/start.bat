cd nginx
start .\nginx
start http://localhost:8021/
echo "服务运行中，按任意键结束游戏"
@echo off
pause
.\nginx -s stop
