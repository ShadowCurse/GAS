--Notifications
listen mymessage;
create or replace function resource_notify()
  returns trigger as
$TRIGGER_R$
    begin
        notify mymessage, 'Resource update';
        return NULL;
    end ;
$TRIGGER_R$
  language plpgsql volatile
  cost 100;
alter function resource_notify() owner to postgres;

create trigger trigger_resource
  after insert or update or delete
  on Resource
  for each row
  execute procedure resource_notify();


create or replace function commits_notify()
  returns trigger as
$TRIGGER_C$
    begin
        notify mymessage, 'Commits update';
        return NULL;
    end ;
$TRIGGER_C$
  language plpgsql volatile
  cost 100;
alter function commits_notify() owner to postgres;

create trigger trigger_commits
  after insert or update or delete
  on Commits
  for each row
  execute procedure commits_notify();