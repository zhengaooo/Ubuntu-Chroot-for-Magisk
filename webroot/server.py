# main.py
from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles
from fastapi.responses import FileResponse
from pydantic import BaseModel
import subprocess

app = FastAPI()

# 挂载静态文件
app.mount("/assets", StaticFiles(directory="assets"), name="static")

@app.get("/")
async def read_index():
    return FileResponse("index.html")

class CMD(BaseModel):
    cmd: str

class RET(BaseModel):
    exit_code: int
    output: str


@app.post("/api")
async def execute_cmd(data: CMD):
    print(data.cmd)
    ret = subprocess.run(data.cmd,capture_output=True, text=True, shell=True)
    print(ret)
    return RET(exit_code=ret.returncode, output=ret.stdout+ret.stderr)

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app,host="0.0.0.0", port=8001)

