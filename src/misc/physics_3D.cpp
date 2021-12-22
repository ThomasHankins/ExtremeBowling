#include "physics_3D.h"

Collider3D::Collider3D()
{
    collider_type = Shape::none;
    p_pos = NULL;
    p_rot = NULL;
    size_x = 0;
    size_y = 0;
    size_z = 0;
    offset = Vec3D(0, 0, 0);
    old_pos = Point3D(0, 0, 0);
}

Collider3D::Collider3D(Shape col, Point3D *p_position, Rot3D *p_rotation, float s_x, float s_y, float s_z, float off_x, float off_y, float off_z)
{
    collider_type = col;
    p_pos = p_position;
    p_rot = p_rotation;
    size_x = s_x;
    size_y = s_y;
    size_z = s_z;
    offset = Vec3D(off_x, off_y, off_z);
    old_pos = calculatePos();
}

Point3D Collider3D::calculatePos()
{
    Vec3D rot_offset = offset.clone();
    p_rot->rotate3D(&rot_offset);
    return rot_offset.movePoint(*p_pos);
}

Vec3D Collider3D::collide(Collider3D col)
{
    if (p_pos == 0 || p_rot == 0 || col.p_pos == 0 || col.p_rot == 0)
        return Vec3D();

    if (collider_type == Shape::box)
    {
        if (col.collider_type == Shape::box)
            return collisionBoxBox(*this, col);
        else
            return collisionBoxSphere(*this, col);
    }
    else if (col.collider_type == Shape::box)
    {
        return collisionBoxSphere(col, *this).multiply(-1);
    }
    return collisionSphereSphere(*this, col);
}


Vec3D Collider3D::collisionPointSphere(Point3D p, Point3D sph_p, float rad)
{
    Vec3D p_to_s = Vec3D::createVector(p, sph_p);
    if (p_to_s.quickLength() < rad * rad)
        return p_to_s.normalize().multiply(rad - p_to_s.length());
    return Vec3D();
}

Vec3D Collider3D::collisionCornerSphere(Point3D corner, Vec3D edge_x, Vec3D edge_y, Vec3D edge_z, Point3D sph_p, float rad)
{
    Vec3D c_to_s = Vec3D::createVector(corner, sph_p);

    float rad_2 = rad * rad;

    // check edge parallel to x axis
    Vec3D proj_x = edge_x.project(c_to_s);
    Vec3D x_to_s = c_to_s.addVec(proj_x.multiply(-1));
    if (x_to_s.quickLength() < rad_2 && c_to_s.dotProd(edge_x) >= 0 && proj_x.quickLength() < edge_x.quickLength())
        return x_to_s.normalize().multiply(rad - x_to_s.length());
    
    // check edge parallel to y axis
    Vec3D proj_y = edge_y.project(c_to_s);
    Vec3D y_to_s = c_to_s.addVec(proj_y.multiply(-1));
    if (y_to_s.quickLength() < rad_2 && c_to_s.dotProd(edge_y) >= 0 && proj_y.quickLength() < edge_y.quickLength())
        return y_to_s.normalize().multiply(rad - y_to_s.length());
    
    // check edge parallel to z axis
    Vec3D proj_z = edge_z.project(c_to_s);
    Vec3D z_to_s = c_to_s.addVec(proj_z.multiply(-1));
    if (z_to_s.quickLength() < rad_2 && c_to_s.dotProd(edge_z) >= 0 && proj_z.quickLength() < edge_z.quickLength())
        return z_to_s.normalize().multiply(rad - z_to_s.length());

    return Vec3D();
}

Vec3D Collider3D::collisionBoxBox(Collider3D box_1, Collider3D box_2)
{
    // check distance
    // check each corner on each box
    Point3D pos_1 = box_1.calculatePos();
    Point3D pos_2 = box_2.calculatePos();
    Vec3D b_to_b = Vec3D::createVector(pos_1, pos_2);
    float contact_length = max(max(box_1.size_x, box_1.size_y), box_1.size_z) + max(max(box_2.size_x, box_2.size_y), box_2.size_z);
    if (b_to_b.quickLength() < ZERO)
        return Vec3D(1, 1, 1);
    if (b_to_b.length() < contact_length)
        return b_to_b;
    return Vec3D();
}
Vec3D Collider3D::collisionBoxSphere(Collider3D box, Collider3D sph)
{
    Point3D pos_box = box.calculatePos();
    Point3D pos_sph = sph.calculatePos();
    Vec3D b_to_s = Vec3D::createVector(pos_box, pos_sph);

    float rad = sph.size_x / 2;

    float contact_length = rad + max(max(box.size_x, box.size_y), box.size_z);
    if (b_to_s.length() < contact_length)
    {
        if (b_to_s.length() <= ZERO)
            return Vec3D(1, 1, 1);

        // get axis for box
        Vec3D x_axis = Vec3D(box.size_x / 2, 0, 0);
        box.p_rot->rotate3D(&x_axis);
        Vec3D y_axis = Vec3D(0, box.size_y / 2, 0);
        box.p_rot->rotate3D(&y_axis);
        Vec3D z_axis = Vec3D(0, 0, box.size_z / 2);
        box.p_rot->rotate3D(&z_axis);

        float delta_x = x_axis.project(b_to_s).length();
        float delta_y = y_axis.project(b_to_s).length();
        float delta_z = z_axis.project(b_to_s).length();

        // check if colliding
        // sphere center is within box
        if (delta_x < box.size_x / 2 && delta_y < box.size_y / 2 && delta_z < box.size_z / 2)
        {
            // complicated reflection - need to find where previous sphere position was
            // use closest face
            float max_delta = max(max(delta_x, delta_y), delta_z);
            if (max_delta == delta_x)
            {
                if (x_axis.dotProd(b_to_s) > 0)
                    return x_axis.normalize().multiply(rad + box.size_x / 2 - delta_x);
                else
                    return x_axis.normalize().multiply(delta_x - rad - box.size_x / 2);
            }
            else if (max_delta == delta_y)
            {
                if (y_axis.dotProd(b_to_s) > 0)
                    return y_axis.normalize().multiply(rad + box.size_y / 2 - delta_y);
                else
                    return y_axis.normalize().multiply(delta_y - rad - box.size_y / 2);
            }
            else if (max_delta == delta_z)
            {
                if (z_axis.dotProd(b_to_s) > 0)
                    return z_axis.normalize().multiply(rad + box.size_z / 2 - delta_z);
                else
                    return z_axis.normalize().multiply(delta_z - rad - box.size_z / 2);
            }
            return y_axis.normalize().multiply(rad + (box.size_y / 2) - delta_y);
        }
        
        // if sphere center is within box on two axis
        if (delta_x < box.size_x / 2 && delta_y < box.size_y / 2) // front/back
        {
            if (delta_z < rad + box.size_z / 2)
            {
                if (z_axis.dotProd(b_to_s) > 0)
                    return z_axis.normalize().multiply(rad + box.size_z / 2 - delta_z);
                else
                    return z_axis.normalize().multiply(delta_z - rad - box.size_z / 2);
            }
                
            return Vec3D();
        }
        if (delta_x < box.size_x / 2 && delta_z < box.size_z / 2)
        {
            if (delta_y < rad + box.size_y / 2) // top/bottom
            {
                if (y_axis.dotProd(b_to_s) > 0)
                    return y_axis.normalize().multiply(rad + box.size_y / 2 - delta_y);
                else
                    return y_axis.normalize().multiply(delta_y - rad - box.size_y / 2);
            }
                
            return Vec3D();
        }
        if (delta_y < box.size_y / 2 && delta_z < box.size_z / 2)
        {
            if (delta_x < rad + box.size_x / 2) // left/right
            {
                if (x_axis.dotProd(b_to_s) > 0)
                    return x_axis.normalize().multiply(rad + box.size_x / 2 - delta_x);
                else
                    return x_axis.normalize().multiply(delta_x - rad - box.size_x / 2);
            }
            return Vec3D();
        }
        
        // calculate 8 corners
        Point3D f_t_r = x_axis.addVec(y_axis.addVec(z_axis)).movePoint(pos_box);
        Point3D f_t_l = x_axis.multiply(-2).movePoint(f_t_r);
        Point3D f_b_l = y_axis.multiply(-2).movePoint(f_t_l);
        Point3D f_b_r = x_axis.multiply(2).movePoint(f_b_l);
        Point3D b_b_r = z_axis.multiply(-2).movePoint(f_b_r);
        Point3D b_b_l = x_axis.multiply(-2).movePoint(b_b_r);
        Point3D b_t_l = y_axis.multiply(2).movePoint(b_b_l);
        Point3D b_t_r = x_axis.multiply(2).movePoint(b_t_l);

        Vec3D ref_norm;
        // check for collision with box edges
        if (delta_x < box.size_x / 2 || delta_y < box.size_y / 2 || delta_z < box.size_z / 2)
        {
            ref_norm = collisionCornerSphere(b_b_l, x_axis.multiply(2), y_axis.multiply(2), z_axis.multiply(2), pos_sph, rad);
            if (ref_norm.quickLength() > 0)
                return ref_norm;
            ref_norm = collisionCornerSphere(b_t_r, x_axis.multiply(-2), y_axis.multiply(-2), z_axis.multiply(2), pos_sph, rad);
            if (ref_norm.quickLength() > 0)
                return ref_norm;
            ref_norm = collisionCornerSphere(f_t_l, x_axis.multiply(2), y_axis.multiply(-2), z_axis.multiply(-2), pos_sph, rad);
            if (ref_norm.quickLength() > 0)
                return ref_norm;
            return collisionCornerSphere(f_b_r, x_axis.multiply(-2), y_axis.multiply(2), z_axis.multiply(-2), pos_sph, rad);
        }

        // check collision with corners
        ref_norm = collisionPointSphere(f_t_r, pos_sph, rad);
        if(ref_norm.quickLength() > 0)
            return ref_norm;
        ref_norm = collisionPointSphere(f_t_l, pos_sph, rad);
        if(ref_norm.quickLength() > 0)
            return ref_norm;
        ref_norm = collisionPointSphere(f_b_l, pos_sph, rad);
        if(ref_norm.quickLength() > 0)
            return ref_norm;
        ref_norm = collisionPointSphere(f_b_r, pos_sph, rad);
        if(ref_norm.quickLength() > 0)
            return ref_norm;
        ref_norm = collisionPointSphere(b_b_r, pos_sph, rad);
        if(ref_norm.quickLength() > 0)
            return ref_norm;
        ref_norm = collisionPointSphere(b_b_l, pos_sph, rad);
        if(ref_norm.quickLength() > 0)
            return ref_norm;
        ref_norm = collisionPointSphere(b_t_l, pos_sph, rad);
        if(ref_norm.quickLength() > 0)
            return ref_norm;
        return collisionPointSphere(b_t_r, pos_sph, rad);
    }
    return Vec3D();
}
Vec3D Collider3D::collisionSphereSphere(Collider3D sph_1, Collider3D sph_2)
{
    Point3D pos_1 = sph_1.calculatePos();
    Point3D pos_2 = sph_2.calculatePos();
    Vec3D reflection_normal = Vec3D::createVector(pos_1, pos_2);
    float contact_length = sph_1.size_x / 2.0f + sph_2.size_x / 2.0f;
    float len = reflection_normal.length();
    if (len < ZERO)
        return Vec3D(1, 1, 1);
    if (len < contact_length)
        return reflection_normal.normalize().multiply(contact_length - len); // reflection normal is also the length needed to push them apart
    return Vec3D();
}

PhysicsObject3D::PhysicsObject3D()
{
    pos = Point3D();
    vel = Vec3D();
    acc = Vec3D();
    rot = Rot3D();
    rot_vel = 0;

    interaction = Reaction::fixed;
    surface_friction = 0;
    acc_friction = 0;

    id = 0;
    local_id = 0;
    collider = Collider3D();
    collided.clear(); 
}

PhysicsObject3D::PhysicsObject3D(float p_x, float p_y, float p_z, int i, float f)
{
    pos = Point3D(p_x, p_y, p_z);
    vel = Vec3D();
    acc = Vec3D();
    rot = Rot3D();
    rot_vel = 0;

    if (i < 0 || i > 2)
        i = 1;
    interaction = static_cast<Reaction>(i);
    surface_friction = f;
    acc_friction = 0;

    id = 0;
    local_id = 0;
    collider = Collider3D();
    collided.clear(); 
}
float PhysicsObject3D::getX()
{
    return pos.x;
}

float PhysicsObject3D::getY()
{
    return pos.y;
}

float PhysicsObject3D::getZ()
{
    return pos.z;
}

Point3D PhysicsObject3D::getPos()
{
    return pos;
}

Vec3D PhysicsObject3D::getVel()
{
    return vel;
}

Rot3D PhysicsObject3D::getRot()
{
    return rot;
}

Reaction PhysicsObject3D::getInteraction()
{
    return interaction;
}

bool PhysicsObject3D::isMoveable()
{
    return interaction == Reaction::kinetic;
}

float PhysicsObject3D::getSurfaceFriction()
{
    return surface_friction;
}

int PhysicsObject3D::getId()
{
    return id;
}

int PhysicsObject3D::getLocalId()
{
    return local_id;
}

vector<Collision> PhysicsObject3D::getCollided()
{
    return collided;
}

void PhysicsObject3D::setId(int object_id)
{
    id = object_id;
}

void PhysicsObject3D::setLocalId(int object_id)
{
    local_id = object_id;
}

void PhysicsObject3D::setPosition(float x, float y, float z)
{
    pos = Point3D(x, y ,z);
}

void PhysicsObject3D::setVelocity(Vec3D v)
{
    vel = v;
}

void PhysicsObject3D::setVelocity(float x, float y, float z)
{
    setVelocity(Vec3D(x, y ,z));
}

void PhysicsObject3D::setRotation(float x, float y, float z, float a)
{
    rot = Rot3D(x, y, z, a);
}

void PhysicsObject3D::addRotation(float x, float y, float z, float a)
{
    rot.addRotation(x, y, z, a, false);
}
void PhysicsObject3D::addRelativeRotation(float x, float y, float z, float a)
{
    rot.addRotation(x, y, z, a, true);
}

void PhysicsObject3D::setRotVel(float r_v)
{
    rot_vel = r_v;
}

void PhysicsObject3D::setInteraction(Reaction r)
{
    interaction = r;
}

void PhysicsObject3D::setSurfaceFriction(float f)
{
    surface_friction = f;
}

void PhysicsObject3D::setAccFriction(float f)
{
    acc_friction = f;
}

void PhysicsObject3D::addBoxCollider(float s_x, float s_y, float s_z, float off_x, float off_y, float off_z)
{
    collider = Collider3D(Shape::box, &pos, &rot, s_x, s_y, s_z, off_x, off_y, off_z);
}

void PhysicsObject3D::addCubeCollider(float size, float off_x, float off_y, float off_z)
{
    collider = Collider3D(Shape::box, &pos, &rot, size, size, size, off_x, off_y, off_z);
}

void PhysicsObject3D::addSphereCollider(float size, float off_x, float off_y, float off_z)
{
    collider = Collider3D(Shape::sphere, &pos, &rot, size, size, size, off_x, off_y, off_z);
}

void PhysicsObject3D::addVelocity(float x, float y, float z)
{
    vel = vel.addVec(Vec3D(x, y, z));
}

void PhysicsObject3D::addAcceleration(float x, float y, float z)
{
    acc = acc.addVec(Vec3D(x, y, z));
}

void PhysicsObject3D::addCallback(int i, int(*f)(void*, Vec3D, void*), void* context)
{
    callbacks.insert(pair<int, Callback>(i, Callback(f, context)));
}

void PhysicsObject3D::removeCallback(int i)
{
    callbacks.erase(i);
}

// updatePhysics
// friction: applies a friction force slowing down the object. set to 0 if not wanted
// time: time passed since last call in ms
void PhysicsObject3D::updatePhysics(float time, bool gravity, vector<PhysicsObject3D *> &objs)
{
    time = time / 1000;     // convert to seconds

    // ------------------- deceleration due to friction -------------------------
    acc_friction = min(acc_friction * time, 0.75f);

    vel = vel.multiply(1-acc_friction); // calculate change in velocity

    acc_friction = 0;

    // --------------------------- kinematics ------------------------------------
    if (gravity)    // add gravity
        acc = acc.addVec(Vec3D(0.0f, GRAVITY, 0.0f));

    vel = vel.addVec(acc.multiply(time));    // add acc to vel

    // apply speed limit to avoid objects passing through each other when moving at high speed
    if (vel.length() > MAX_SPD)
        vel = vel.multiply(MAX_SPD / vel.length());
    
    // add vel to position
    pos = vel.multiply(time).movePoint(pos);

    // reset acceleration
    acc = Vec3D();

    // ------------------------ collisions with other objects -------------------------
    if (collider.collider_type != Shape::none)
    {
        for (vector<PhysicsObject3D *>::iterator it = objs.begin(); it < objs.end(); it++)
        {
            collision(*it);
        }

    }
}

void PhysicsObject3D::reflect(Vec3D ref_normal, float scale)
{
    vel = vel.addVec(ref_normal.project(vel).multiply(-2)).multiply(scale);
}

void PhysicsObject3D::collision(PhysicsObject3D *other_obj)
{
    // ref_normal is normal of reflection plane that both objects should move away from
    // this means it is the direction the two objects should move in order to move away from each other
    // the length of ref_normal is the length the two objects should be separated by in order to stop overlapping
    Vec3D ref_normal = collider.collide(other_obj->collider);

    if (ref_normal.length() == 0)
        return;

    // check reaction type
    bool bounce_1 = isMoveable() && other_obj->getInteraction() != Reaction::ghost;
    bool bounce_2 = other_obj->isMoveable() && getInteraction() != Reaction::ghost;

    // run callbacks and get reaction override
    // -1 means force no collision
    //  0 means use default collision
    //  1 means force collision
    int force_1 = runCallback(other_obj->getId(), ref_normal.multiply(-1), other_obj);
    int force_2 = other_obj->runCallback(getId(), ref_normal, this);
    
    // compute reaction types for both objects
    bounce_1 = (bounce_1 && force_1 == 0) || force_1 == 1;
    bounce_2 = (bounce_2 && force_2 == 0) || force_2 == 1;

    // save projection of original velocities going towards each other
    Vec3D vel_1 = ref_normal.project(vel);
    Vec3D vel_2 = ref_normal.project(other_obj->vel);

    // calculate friction
    float collision_friction = other_obj->getSurfaceFriction() * surface_friction;
    acc_friction = max(collision_friction, acc_friction);
    other_obj->setAccFriction(collision_friction);

    // two collision
    if (bounce_1 && bounce_2)
    {
        pos = ref_normal.multiply(-0.5).movePoint(pos);
        other_obj->pos = ref_normal.multiply(0.5).movePoint(other_obj->pos);

        vel = vel.addVec(vel_1.multiply(-1)).addVec(vel_2);
        other_obj->vel = other_obj->vel.addVec(vel_2.multiply(-1)).addVec(vel_1);
    }
    // single collision - ie collision with an immovable object
    else if (bounce_1)
    {
        pos = ref_normal.multiply(-1).movePoint(pos);
        if (ref_normal.dotProd(vel) > 0)
            vel = vel.addVec(vel_1.multiply(-1.5 + collision_friction));
        vel = vel.addVec(vel_2);
    }
    else if (bounce_2)
    {
        other_obj->pos = ref_normal.movePoint(other_obj->pos);
        if (ref_normal.dotProd(other_obj->vel) < 0)
            other_obj->vel = other_obj->vel.addVec(vel_2.multiply(-1.5 + collision_friction));
        other_obj->vel = other_obj->vel.addVec(vel_1);
    }
}

int PhysicsObject3D::runCallback(int id, Vec3D deflection, void* obj)
{
    // run callback function
    map<int, Callback>::iterator it;
    it = callbacks.find(id);

    if (it != callbacks.end())
        return it->second.runFunction(deflection, obj);
    
    return 0;
}