from persistence import Activitie, repo
import sys

def main(args):
    inputfilename = args[1]
    with open(inputfilename) as inputfile:
        for line in inputfile:
            product_id, quantity, activator_id, date = map(str.strip, line.split(","))
            product = repo.products.find(id=int(product_id))
            
            if product:
                product = product[0]
                quantity = int(quantity)
                activator_id = int(activator_id)

                if quantity > 0 or (quantity < 0 and product.quantity >= abs(quantity)):
                    repo.activities.insert(Activitie(int(product_id), quantity, activator_id, date))

                    new_quantity = product.quantity + quantity
                    repo._conn.execute("UPDATE products SET quantity = ? WHERE id = ?", 
                                       (new_quantity, product_id))

if __name__ == '__main__':
    main(sys.argv)
