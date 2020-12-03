insert into logs(date, description) values ('2020-10-10', 'test_log0');
insert into logs(date, description) values ('2020-10-10', 'test_log1');
insert into logs(date, description) values ('2020-10-10', 'test_log2');
insert into logs(date, description) values ('2020-10-10', 'test_log3');

insert into users(username, password, email, description) values ('user1', 'password1', 'email1', 'description1');
insert into users(username, password, email, description) values ('user2', 'password2', 'email2', 'description2');
insert into users(username, password, email, description) values ('user3', 'password3', 'email3', 'description3');
insert into users(username, password, email, description) values ('user4', 'password4', 'email4', 'description4');

insert into resourcetype(name) values ('Texture');
insert into resourcetype(name) values ('Mesh');
insert into resourcetype(name) values ('Audio');
insert into resourcetype(name) values ('TileMap');
insert into resourcetype(name) values ('Animation');
insert into resourcetype(name) values ('Level');
insert into resourcetype(name) values ('Sprite');
insert into resourcetype(name) values ('Particle');

insert into resource(name, description, size, type, data) values ('Texture1', 'Descr1', 345, 1, '123132131');
insert into resource(name, description, size, type, data) values ('Texture2', 'Descr2', 345, 1, '123132131');
insert into resource(name, description, size, type, data) values ('Texture3', 'Descr3', 345, 1, '123132131');
insert into resource(name, description, size, type, data) values ('Texture4', 'Descr4', 345, 1, '123132131');
insert into resource(name, description, size, type, data) values ('Texture5', 'Descr5', 345, 1, '123132131');
insert into resource(name, description, size, type, data) values ('Texture6', 'Descr6', 345, 1, '123132131');
insert into resource(name, description, size, type, data) values ('Texture7', 'Descr7', 345, 1, '123132131');
insert into resource(name, description, size, type, data) values ('Mesh1', 'Very important mesh', 345, 2, '123132131');
insert into resource(name, description, size, type, data) values ('Audio1', 'Descr4', 4, 3, '123132131');

insert into dependency(requesting_resource, required_resource) values (8, 1);
insert into dependency(requesting_resource, required_resource) values (8, 2);
insert into dependency(requesting_resource, required_resource) values (8, 3);
insert into dependency(requesting_resource, required_resource) values (8, 4);
insert into dependency(requesting_resource, required_resource) values (8, 5);
insert into dependency(requesting_resource, required_resource) values (8, 6);

insert into commits("user", resource, date, message) values (1, 1, '2020-10-10', 'Commit1');
insert into commits("user", resource, date, message) values (1, 2, '2020-10-10', 'Commit2');
insert into commits("user", resource, date, message) values (1, 3, '2020-10-10', 'Commit3');
insert into commits("user", resource, date, message) values (1, 3, '2020-10-10', 'Commit4');
