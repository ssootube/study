import win32clipboard

#클립보드를 하나의 파일처럼 생각하면 이해하기 쉽다.

win32clipboard.OpenClipboard() #클립보드를 열고,
win32clipboard.EmptyClipboard() #클립보드 내용을 다 지우고
win32clipboard.SetClipboardText('test text') #원하는 내용으로 채워넣는다.
win32clipboard.CloseClipboard() #그리고 클립보드를 닫는다.


win32clipboard.OpenClipboard() #클립보드를 열고
clipboard = win32clipboard.GetClipboardData() #클립보드 내부 데이터를 가져온다.
win32clipboard.CloseClipboard() #클립보드를 닫는다.

print(clipboard)
