from flask import Flask, render_template_string
import sqlite3

app = Flask(__name__)

HTML = """
<h2>Jobs Table</h2>
<table border="1" cellpadding="5">
<tr>
{% for col in columns %}
    <th>{{ col }}</th>
{% endfor %}
</tr>
{% for row in rows %}
<tr>
    {% for cell in row %}
        <td>{{ cell }}</td>
    {% endfor %}
</tr>
{% endfor %}
</table>
"""

@app.route("/")
def show_table():
    conn = sqlite3.connect("example.db")
    cur = conn.cursor()
    cur.execute("SELECT * FROM JOBS")
    rows = cur.fetchall()
    columns = [desc[0] for desc in cur.description]
    conn.close()
    return render_template_string(HTML, rows=rows, columns=columns)

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)  # visible on LAN