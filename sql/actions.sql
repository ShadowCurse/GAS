--################## logs ########################
insert into logs(date, description) values ('10-10-2010', 'test_log');
select * from logs; -- select all logs
select * from logs where date between '10-10-2010' and '11-11-2011'; -- select between dates
truncate table logs; -- clears all logs

--################## developer ########################
insert into developer(name, email, description) values ('name', 'email', 'description');
select * from developer; -- selecting all developers
select * from developer where id = 0; -- selecting particular developer
update developer set name = 'name', email = 'email', description = 'description' where id = 0;
delete from developer where id = 1;

--################## resourcetype ########################
insert into resourcetype(name) values ('Texture');
select * from resourcetype; -- select all types
select * from resourcetype where id = 0; -- select one type
update resourcetype set name = 'Mesh' where id = 0;
delete from resourcetype where id = 0;

--################## resource ########################
insert into resource(name, description, size, checksum, type, data) values ('Level1', 'Descr', 12, 'A0EEBC99-9C0B-4EF8-BB6D-6BB9BD380A11',
                                                                            0, '123132131');
select * from resource; -- selecting all resources
select * from resource where id = 0; -- selecting resource by id
update resource set name = 'Texture1', description = 'Descr', size = 12,
                    checksum = 'a0eebc99-9c0b-4ef8-bb6d-6bb9bd380a11', type = 1, data = 2321231 where id = 2;
delete from resource where id = 1;

--################## dependency ########################
insert into dependency(requesting_resource, required_resource) values (0, 1);
select * from dependency; -- selecting all dependencies
select * from dependency where requesting_resource = 0; -- selecting dependencies of resource
-- selecting id and names for resources
select  dependency.id, Requesting.name, Required.name from dependency join resource Requesting on
        dependency.requesting_resource = Requesting.id join resource Required on dependency.required_resource=Required.id;
update  dependency set requesting_resource = 3, required_resource = 2 where id = 1;
delete from dependency where requesting_resource = 0 and required_resource = 1; -- delete one dependency
delete from dependency where id = 1; -- delete all dependencies of the resource

--################## commit ########################
insert into commits(dev, resource, date, message) values (0, 0, '2010-10-10', 'Added saving');
select * from  commits; -- select all commits
select * from commits where resource = 0; -- select commits on resource
select * from commits where dev = 0; -- select commits of dev
select * from commits where dev = 0 and resource = 0; -- select commits of dev on resource
update commits set dev = 8, resource = 2, date = '2012-10-10', message = 'Add new material' where id = 4;
delete from commits where id = 1;
