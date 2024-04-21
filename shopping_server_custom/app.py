from flask import Flask, render_template, request
import sqlite3

con = sqlite3.connect('shopping.db')
cr = con.cursor()

cr.execute('''CREATE TABLE product_details (
	product_id INTEGER PRIMARY KEY AUTOINCREMENT,
	product_name TEXT,
	product_price NUMERIC,
	product_weight NUMERIC
    ''')
cr.execute("create table if not exists products(Id INTEGER PRIMARY KEY AUTOINCREMENT, rfid TEXT, name TEXT, weight TEXT, price TEXT)")
cr.execute("create table if not exists customer(Id INTEGER PRIMARY KEY AUTOINCREMENT, phone TEXT, rfid TEXT, name TEXT, weight TEXT, price TEXT)")

app = Flask(__name__)

@app.route('/')
def index():
    con = sqlite3.connect('shopping.db')
    cr = con.cursor()
    cr.execute("select rfid, name, weight, price from products")
    result = cr.fetchone()
    print(result)
    return render_template('index.html')

@app.route('/update', methods=['POST', 'GET'])
def update():
    if request.method == 'POST':
        con = sqlite3.connect('shopping.db')
        cr = con.cursor()

        rfid  = request.form['rfid']
        weight = request.form['weight']
        price = request.form['price']
        name = request.form['name']

        data = [rfid, name, weight, price]
        print(data)

        cr.execute('insert into products(rfid, name, weight, price) values (?,?,?,?)', data)
        con.commit()

        return render_template('index.html', msg='updated successfully')

    return render_template('index.html')


@app.route('/generate')
def generate():
    from serial_test import scan_phone, scan_weight
    phone = scan_phone()
    detected_rfid, loadcel_weight = scan_weight()

    data = []
    actual_weight = 0
    detected_weight = 0
    total_price = 0
    for i in range(len(detected_rfid)):
        con = sqlite3.connect('shopping.db')
        cr = con.cursor()

        cr.execute("select rfid, name, weight, price from products where rfid = '"+detected_rfid[i]+"'")
        result = cr.fetchone()
        rfid, name, weight, price = result
        
        actual_weight += float(weight)
        detected_weight += float(loadcel_weight[i])

        total_price += float(price)
        
        data.append([phone, detected_rfid[i], name, loadcel_weight[i], price])
        cr.execute('insert into customer(phone, rfid, name, weight, price) values (?,?,?,?,?)', [phone, detected_rfid[0], name, loadcel_weight[0], price])
        con.commit()
    

    print('done............')
    return render_template('index.html', data = data, total_price='{:.2f}'.format(total_price), actual_weight='{:.2f}'.format(actual_weight), detected_weight = '{:.2f}'.format(detected_weight), heading = ['phone', 'rfid', 'name', 'weight', 'price'])

if __name__ == "__main__":
    app.run(debug=True)
