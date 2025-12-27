from persistence import repo

def main():
    print("\nActivities")
    for row in repo.activities.find_all():
        print(f"({row.product_id}, {row.quantity}, {row.activator_id}, '{row.date}')")
    
    print("\nBranches")
    for row in repo.branches.find_all():
        print(f"({row.id}, '{row.location}', {row.number_of_employees})")

    print("\nEmployees")
    for row in repo.employees.find_all():
        print(f"({row.id}, '{row.name}', {row.salary}, {row.branche})")

    print("\nProducts")
    for row in repo.products.find_all():
        print(f"({row.id}, '{row.description}', {row.price}, {row.quantity})")

    print("\nSuppliers")
    for row in repo.suppliers.find_all():
        print(f"({row.id}, '{row.name}', '{row.contact_information}')")

    print("\nEmployees report")
    branches = {b.id: b.location for b in repo.branches.find_all()}
    
    for row in sorted(repo.employees.find_all(), key=lambda e: e.name):
        total_sales = sum(
            activity.quantity * repo.products.find(id=activity.product_id)[0].price
            for activity in repo.activities.find(activator_id=row.id)
            if activity.quantity < 0
        )
        print(f"{row.name} {row.salary} {branches[row.branche]} {total_sales}")

    print("\nActivities report")
    activities = sorted(repo.activities.find_all(), key=lambda a: a.date)
    
    for activity in activities:
        product = repo.products.find(id=activity.product_id)[0]
        actor = repo.employees.find(id=activity.activator_id) or repo.suppliers.find(id=activity.activator_id)

        actor_name = None
        supplier_name = None

        if actor:
            actor = actor[0]
            if hasattr(actor, "name"):
                actor_name = actor.name  
            elif hasattr(actor, "contact_information"):
                supplier_name = actor.name 

        print(f"('{activity.date}', '{product.description}', {activity.quantity}, {actor_name if actor_name else None}, {supplier_name if supplier_name else None})")

if __name__ == '__main__':
    main()
