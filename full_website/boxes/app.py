# app.py (Flask application)

from flask import Flask, render_template, request, jsonify

app = Flask(__name__)

# Initial rectangle and boxes data
rectangle_data = {'width': 500, 'height': 300}
boxes_data = [{'id': 1, 'x': 50, 'y': 50, 'width': 100, 'height': 100}]

@app.route('/')
def index():
    return render_template('index.html', rectangle=rectangle_data, boxes=boxes_data)

@app.route('/add_box', methods=['POST'])
def add_box():
    global boxes_data
    new_id = len(boxes_data) + 1
    boxes_data.append({'id': new_id, 'x': 0, 'y': 0, 'width': 50, 'height': 50})
    return jsonify({'success': True, 'id': new_id})

@app.route('/delete_box/<int:box_id>', methods=['POST'])
def delete_box(box_id):
    global boxes_data
    boxes_data = [box for box in boxes_data if box['id'] != box_id]
    return jsonify({'success': True})

@app.route('/update_box/<int:box_id>', methods=['POST'])
def update_box(box_id):
    global boxes_data
    data = request.get_json()
    for box in boxes_data:
        if box['id'] == box_id:
            box['x'] = data['x']
            box['y'] = data['y']
            box['width'] = data['width']
            box['height'] = data['height']
            break
    return jsonify({'success': True})

if __name__ == '__main__':
    app.run(debug=True)
