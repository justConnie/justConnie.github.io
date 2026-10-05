FROM python:3.12

WORKDIR /app

COPY reqs.txt .
RUN pip install --no-cache-dir -r reqs.txt

COPY main.py .

CMD ["python", "main.py"]
