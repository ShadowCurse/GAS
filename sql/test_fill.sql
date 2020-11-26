insert into logs(date, description) values ('10-10-2010', 'test_log0');
insert into logs(date, description) values ('11-10-2010', 'test_log1');
insert into logs(date, description) values ('12-10-2010', 'test_log2');
insert into logs(date, description) values ('9-10-2010', 'test_log3');

insert into users(username, password, email, description) values ('name1', 'password1', 'email1', 'description1');
insert into users(username, password, email, description) values ('name2', 'password2', 'email2', 'description2');
insert into users(username, password, email, description) values ('name3', 'password3', 'email3', 'description3');
insert into users(username, password, email, description) values ('name4', 'password4', 'email4', 'description4');

insert into resourcetype(name) values ('Texture');
insert into resourcetype(name) values ('Mesh');
insert into resourcetype(name) values ('Audio');
insert into resourcetype(name) values ('TileMap');

insert into resource(name, description, size, type, data) values ('Texture1', 'Descr1', 1, 1, '123132131');
insert into resource(name, description, size, type, data) values ('Texture2', 'Descr2', 2, 1, '123132131');
insert into resource(name, description, size, type, data) values ('Mesh1', 'Descr3', 3, 2, '123132131');
insert into resource(name, description, size, type, data) values ('Audio1', 'Descr4', 4, 3, '123132131');

insert into dependency(requesting_resource, required_resource) values (3, 2);
insert into dependency(requesting_resource, required_resource) values (2, 4);
insert into dependency(requesting_resource, required_resource) values (2, 5);

insert into commits(user, resource, date, message) values (2, 2, '2010-10-10', 'Commit1');
insert into commits(user, resource, date, message) values (4, 2, '2011-10-10', 'Commit2');
insert into commits(user, resource, date, message) values (4, 3, '2012-10-10', 'Commit3');
insert into commits(user, resource, date, message) values (1, 3, '2013-10-10', 'Commit4');
