from flask import Flask, request, jsonify
ORDERS = {}
@app.route('/api/v1/orders/<int:order_id>', methods=['GET'])
def get_order(order_id):
    if order_id not in ORDERS:
        return jsonify({'message': f'Order {order_id} not found'}), 404

    return jsonify(ORDERS[order_id]), 200

@app.route('/api/v1/orders', methods=['POST'])
def create_order():
    new_order = request.json

    order_id = len(ORDERS) + 1
    ORDERS[order_id] = new_order

    return jsonify({'id': order_id, **new_order}), 201

@app.route('/api/v1/orders/<int:order_id>', methods=['DELETE'])
def delete_order(order_id):
    if order_id in ORDERS:
        del ORDERS[order_id]

        return '', 204
    return '', 404

